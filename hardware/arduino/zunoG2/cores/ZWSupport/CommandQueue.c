#include "CommandQueue.h"
#include "Debug.h"

static ZNLinkedList_t * g_zwpkg_queue = NULL;
#ifdef LOGGING_UART
void zuno_dbgdumpZWPacakge(ZUNOCommandPacket_t * cmd);
#endif

bool ZWQPushPackage(ZUNOCommandPacket_t * pkg){
    if(zunoNID() == 0) { // We are out of network - don't send anything
		#ifdef LOGGING_DBG
		LOGGING_UART.print(millis());
		LOGGING_UART.println(" Package was dropped! NodeID==0");
		#endif
		return false;
		
	}
    ZUNOCommandPacket_t * stored_pck;
    stored_pck = (ZUNOCommandPacket_t *)malloc(sizeof(ZUNOCommandPacket_t));
    if (stored_pck == NULL)
        return false;
    memcpy(stored_pck, pkg, sizeof(ZUNOCommandPacket_t));
    // We have to allocated command buffer dynamically too
    // add ZAF offset
    stored_pck->cmd = (uint8_t*)malloc(sizeof(uint8_t)*pkg->len + MAX_ZWTRANSPORT_ENCAP);
    if(stored_pck->cmd == NULL){
        free(stored_pck);
        return false;
    }
    memset(stored_pck->cmd, 0, MAX_ZWTRANSPORT_ENCAP);
    stored_pck->cmd += MAX_ZWTRANSPORT_ENCAP;
    // copy command data from the ZAF offset
    memcpy(stored_pck->cmd , pkg->cmd, pkg->len);
    znllPushBack(&g_zwpkg_queue, (void*)stored_pck);
    #ifdef LOGGING_DBG
	LOGGING_UART.print("\n >>> (");
	LOGGING_UART.print(millis());
	LOGGING_UART.print(") ENQUEUED PACKAGE: ");
	zuno_dbgdumpZWPacakge(pkg);
	#endif
    return true;
}
ZUNOCommandPacket_t * ZWQFindPackage(uint8_t dst_id, uint8_t flags){
    ZNLinkedList_t *e;
    ZUNOCommandPacket_t * p;
    for(e=g_zwpkg_queue;e; e=e->next){
        p = (ZUNOCommandPacket_t *) e->data;
        if(flags != 0xFF){
            if((p->flags & flags)  !=  flags)
                continue;
        }
        if(p->dst_node != dst_id)
            continue;
        return p;
    }
    return NULL;
}
bool zunoExtractGroupNode(uint8_t g, uint8_t i, ZUnoAssocNode_t * node){
	 return zunoSysCall(ZUNO_SYSFUNC_ASSOCIATION_NODE, 3, g, i, node) == 0;
}
bool zunoCheckSystemQueueStatus(uint8_t channel){
    uint32_t interval = millis() - g_zuno_sys->rstat_pkgs_hp_time;
    if((channel > 0) && (interval < SYSTEM_PKG_DOMINATION_TIME)){
        #ifdef LOGGING_DBG
		LOGGING_UART.print("*** HIGH PRIORITY PKG DOMINATION. INTERVAL:");
        LOGGING_UART.println(interval);
        #endif
        return true;
    }
    bool  b_busy = (g_zuno_sys->rstat_priority_counts[channel] > 0);
    #ifdef LOGGING_DBG
    if(b_busy){
	    LOGGING_UART.print("*** QUEUE CHANNEL is BUSY:");
        LOGGING_UART.println(channel);
    }
    #endif
    return b_busy;
}
void _ZWQSend(ZUNOCommandPacket_t * p){
    #ifdef LOGGING_DBG
	LOGGING_UART.print("\n >>> (");
	LOGGING_UART.print(millis());
	LOGGING_UART.print(") OUTGOING PACKAGE: ");
	zuno_dbgdumpZWPacakge(p);
	#endif
    if(p->src_zw_channel & ZWAVE_CHANNEL_MAPPED_BIT){
		p->src_zw_channel = 0;
		zunoSysCall(ZUNO_SYSFUNC_SENDPACKET, 1, p);
		p->src_zw_channel &= ~(ZWAVE_CHANNEL_MAPPED_BIT);
	}
    zunoSysCall(ZUNO_SYSFUNC_SENDPACKET, 1, p); 
}
void _ZWQRemovePkg(ZUNOCommandPacket_t * p){
    // revert back cmd addr
    p->cmd -= MAX_ZWTRANSPORT_ENCAP;
    free(p->cmd);
    free(p);
}
bool ZWQIsEmpty(){
    return (znllCount(g_zwpkg_queue) == 0);
}

void ZWQProcess(){
    ZNLinkedList_t *e;
    ZUnoAssocNode_t node;
    
    // Walk through the queue
    int qi;
    int processed_indexes[MAX_PROCESSED_QUEUE_PKGS];
    int processed_indexes_cnt = 0;
    ZUNOCommandPacket_t * p;
    // Process the packages
    #ifdef LOGGING_DBG
    int queue_sz = znllCount(g_zwpkg_queue);
    if(queue_sz > 0){
	    LOGGING_UART.print("CommandQueue Size:");
        LOGGING_UART.println(queue_sz);
    }
    #endif
    for(e=g_zwpkg_queue, qi=0;e; e=e->next, qi++){
        p = (ZUNOCommandPacket_t *) e->data;
       
        uint32_t system_queue_count = g_zuno_sys->rstat_pkgs_queued - g_zuno_sys->rstat_pkgs_processed; //s.pkgs_queued - s.pkgs_processed;
        if(system_queue_count >= MAX_SYS_QUEUE_PKGS){
            #ifdef LOGGING_DBG
		    LOGGING_UART.print("*** SYSTEM QUEUE IS FULL. CNT:");
            LOGGING_UART.println(system_queue_count);
            LOGGING_UART.print("  QUEUED:");
            LOGGING_UART.print(g_zuno_sys->rstat_pkgs_queued);
            LOGGING_UART.print("  PROCESSED:");
            LOGGING_UART.println(g_zuno_sys->rstat_pkgs_processed);
            #endif
            break;
        }
        uint8_t q_ch = p->flags & ZUNO_PACKETFLAGS_PRIORITY_MASK; 
        // Check if we have a free channel to send the package
        if(zunoCheckSystemQueueStatus(q_ch))
            continue; // Needed queue is full for this priority - just skip the package
        #ifdef LOGGING_DBG
		LOGGING_UART.print("*** QCH:");
        LOGGING_UART.println(q_ch);
        #endif
        if(p->flags & ZUNO_PACKETFLAGS_GROUP){
            #ifdef LOGGING_DBG
		    LOGGING_UART.print(millis());
		    LOGGING_UART.print("*** GROUP:");
            LOGGING_UART.print(p->dst_node);
            LOGGING_UART.print(" I:");
            LOGGING_UART.print(p->dst_zw_channel);
            LOGGING_UART.println(" PKG");
		    #endif
            // Send that packet to group
            // Extract node from association storage
            zunoExtractGroupNode(p->dst_node, p->dst_zw_channel, &node);
            #ifdef LOGGING_DBG
		    LOGGING_UART.print("*** NODE ID:");
            LOGGING_UART.print(node.dest_nodeid);
            LOGGING_UART.print(" channel:");
            LOGGING_UART.println(node.dest_nodeid);
		    #endif
            if(node.dest_nodeid == 0){
                // There is no more data for this group
                processed_indexes[processed_indexes_cnt++] = qi;
                if(processed_indexes_cnt >= MAX_PROCESSED_QUEUE_PKGS)
                    break;
                continue;
            } 
            // Save group attributes
            uint16_t tmp = p->dst_node;
            uint8_t tmp_ch = p->dst_zw_channel;
            uint8_t tmp_flags = p->flags;
            // Copy Node id of specific device
            p->dst_node = node.dest_nodeid;
            p->dst_zw_channel = node.dest_channel;
            p->zw_rx_secure_opts = node.security_level;
            p->flags &= ~(ZUNO_PACKETFLAGS_GROUP);
            // Send package to target device
            _ZWQSend(p);
            // Restore group attributes 
            p->dst_node = tmp;
            p->dst_zw_channel = tmp_ch + 1;
            p->flags = tmp_flags;
        } else {
            _ZWQSend(p);
            processed_indexes[processed_indexes_cnt++] = qi;
            if(processed_indexes_cnt >= MAX_PROCESSED_QUEUE_PKGS)
                break;
            break;
        }
    }
    #ifdef LOGGING_DBG
    if(processed_indexes_cnt){
	    LOGGING_UART.print("*** PROCESSED:");
        LOGGING_UART.println(processed_indexes_cnt);
    }
	#endif
    // Clean queue
    while(processed_indexes_cnt){
        // The backward order, otherwise it will break the indexes
        qi = processed_indexes[processed_indexes_cnt-1];
        #ifdef LOGGING_DBG
	    LOGGING_UART.print("*** CLEANUP:");
        LOGGING_UART.println(qi);
	    #endif
        p = (ZUNOCommandPacket_t *)znllRemove(&g_zwpkg_queue, qi);
        _ZWQRemovePkg(p);
        processed_indexes_cnt--;
    }
}