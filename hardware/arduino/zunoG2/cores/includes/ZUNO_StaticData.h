#ifndef ZUNO_STATIC_DATA_H
#define ZUNO_STATIC_DATA_H

#include "ArduinoTypes.h"
#include "ZUNO_Definitions.h"

#ifdef ZUNO_STATICGATHERING_PHASE
#ifndef NULL
#define NULL 0
#endif
#define VOID_FUNC_POINTER_VOID (void *)
typedef void * GENERIC_POINTER;

// System Datatypes
// -----------------------------------------------------------------
typedef struct _ZUNO_CHANNEL_PROPERTIES_DESCRIPTION
{
	BYTE channel_cmd_class;
	BYTE channel_sensor_type;
	BYTE channel_multilevel_properties;
	GENERIC_POINTER getter1;
	GENERIC_POINTER setter1;
	GENERIC_POINTER getter2;
	GENERIC_POINTER setter2;
} ZUNO_CHANNEL_PROPERTIES_DESCRIPTION;


typedef struct _ZUNO_ASSOCIATION_PROPERTIES_DESCRIPTION
{
	BYTE association_type;
	BYTE association_param;
} ZUNO_ASSOCIATION_PROPERTIES_DESCRIPTION;

#define ZUNO_NO_CHANNEL 														{0, 0, 0, 0, 0}
#define ZUNO_SWITCH_BINARY(GETTER, SETTER)   												{ZUNO_SWITCH_BINARY_CHANNEL_NUMBER, 0, 0, (void*)GETTER, (void*)SETTER, (void*)0, (void*)0}
#define ZUNO_SIREN(GETTER, SETTER)   														{ZUNO_SIREN_CHANNEL_NUMBER, 0, 0, (void*)GETTER, (void*)SETTER,  (void*)0, (void*)0}
#define ZUNO_FLOWSTOP(GETTER, SETTER)   													{ZUNO_FLOWSTOP_CHANNEL_NUMBER, 0, 0, (void*)GETTER, (void*)SETTER,  (void*)0, (void*)0}
#define ZUNO_SWITCH_MULTILEVEL(GETTER, SETTER) 												{ZUNO_SWITCH_MULTILEVEL_CHANNEL_NUMBER, 0, 0, (void*)GETTER, (void*)SETTER,  (void*)0, (void*)0}
#define ZUNO_BLINDS(GETTER, SETTER)															{ZUNO_BLINDS_CHANNEL_NUMBER, 0, 0, (void*)GETTER, (void*)SETTER,  (void*)0, (void*)0}
#define ZUNO_SENSOR_BINARY(TYPE, GETTER) 													{ZUNO_SENSOR_BINARY_CHANNEL_NUMBER, TYPE, ZUNO_NOTIFICATION_TURNED_ON, (void*)GETTER, (void*)MACRO_CAST_POINTER_TO_VOID(0),  (void*)0, (void*)0}
#define ZUNO_SENSOR_MULTILEVEL(TYPE, SCALE, SIZE, PRECISION, GETTER) 						{ZUNO_SENSOR_MULTILEVEL_CHANNEL_NUMBER, TYPE, SENSOR_MULTILEVEL_PROPERTIES_COMBINER(SCALE, SIZE, PRECISION), (void*)GETTER, MACRO_CAST_POINTER_TO_VOID(0),  (void*)0, (void*)0}
#define ZUNO_THERMOSTAT(MODES, UNITS, NEG_POS, LIMITS, GETTER1, SETTER1, GETTER2, SETTER2 )	{ZUNO_THERMOSTAT_CHANNEL_NUMBER, MODES, THERMOSTAT_PROPERTIES_COMBINER(UNITS, NEG_POS, LIMITS), (void*) GETTER1, (void*) SETTER1, (void*) GETTER2, (void*) SETTER2} 
#define ZUNO_SWITCH_COLOR(COLORS, GETTER, SETTER)											{ZUNO_SWITCH_COLOR_CHANNEL_NUMBER, COLORS, 0,    (void*)GETTER, (void*)SETTER, (void*)0, (void*)0} 
#define ZUNO_DOORLOCK(GETTER, SETTER)   													{ZUNO_DOORLOCK_CHANNEL_NUMBER, 0, 0, (void*)GETTER, (void*)SETTER,  (void*)0, (void*)0}
#define ZUNO_METER(TYPE, RESETABLE,  SCALE, SIZE, PRECISION, GETTER, RESETTER) 				{ZUNO_METER_CHANNEL_NUMBER, TYPE | ((RESETABLE&0x01) << 7), METER_PROPERTIES_COMBINER(SCALE, SIZE, PRECISION), (void*)GETTER, (void*)RESETTER,  (void*)0, (void*)0}


#define ZUNO_SENSOR_BINARY_GENERAL_PURPOSE(GETTER)							ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_GENERAL_PURPOSE, GETTER)
#define ZUNO_SENSOR_BINARY_SMOKE(GETTER)									ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_SMOKE, GETTER)
#define ZUNO_SENSOR_BINARY_CO(GETTER)										ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_CO, GETTER)
#define ZUNO_SENSOR_BINARY_CO2(GETTER)										ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_CO2, GETTER)
#define ZUNO_SENSOR_BINARY_HEAT(GETTER)										ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_HEAT, GETTER)
#define ZUNO_SENSOR_BINARY_WATER(GETTER)									ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_WATER, GETTER)
#define ZUNO_SENSOR_BINARY_FREEZE(GETTER)									ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_FREEZE, GETTER)
#define ZUNO_SENSOR_BINARY_TAMPER(GETTER)									ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_TAMPER, GETTER)
#define ZUNO_SENSOR_BINARY_AUX(GETTER)										ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_AUX, GETTER)
#define ZUNO_SENSOR_BINARY_DOOR_WINDOW(GETTER)								ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_DOOR_WINDOW, GETTER)
#define ZUNO_SENSOR_BINARY_TILT(GETTER)										ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_TILT, GETTER)
#define ZUNO_SENSOR_BINARY_MOTION(GETTER)									ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_MOTION, GETTER)
#define ZUNO_SENSOR_BINARY_GLASSBREAK(GETTER)								ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_GLASSBREAK, GETTER)


#define ZUNO_SENSOR_MULTILEVEL_TEMPERATURE(GETTER) 							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_TEMPERATURE, SENSOR_MULTILEVEL_SCALE_CELSIUS, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)	
#define ZUNO_SENSOR_MULTILEVEL_GENERAL_PURPOSE(GETTER) 						ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_GENERAL_PURPOSE_VALUE, SENSOR_MULTILEVEL_SCALE_PERCENTAGE_VALUE, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_LUMINANCE(GETTER) 							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_LUMINANCE, SENSOR_MULTILEVEL_SCALE_LUX, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_POWER(GETTER) 								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_POWER, SENSOR_MULTILEVEL_SCALE_WATT, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_HUMIDITY(GETTER) 							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_RELATIVE_HUMIDITY, SENSOR_MULTILEVEL_SCALE_PERCENTAGE_VALUE, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_VELOCITY(GETTER)								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_VELOCITY, SENSOR_MULTILEVEL_SCALE_METERS_PER_SECOND, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
//#define ZUNO_SENSOR_MULTILEVEL_DIRECTION(GETTER)
#define ZUNO_SENSOR_MULTILEVEL_ATHMOSPHERIC_PRESSURE(GETTER)				ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_ATMOSPHERIC_PRESSURE, SENSOR_MULTILEVEL_SCALE_KILO_PASCAL, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_BAROMETRIC_PRESSURE(GETTER)					ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_BAROMETRIC_PRESSURE, SENSOR_MULTILEVEL_SCALE_KILO_PASCAL, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_SOLAR_RADIATION(GETTER)						ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_SOLAR_RADIATION, SENSOR_MULTILEVEL_SCALE_WAVELENGTH_PER_SQUARE_METER, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_DEW_POINT(GETTER)							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_DEW_POINT, SENSOR_MULTILEVEL_SCALE_CELSIUS, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_RAIN_RATE(GETTER)							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_RAIN_RATE, SENSOR_MULTILEVEL_SCALE_MILLIMETERS_PER_HOUR, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_TIDE_LEVEL(GETTER)							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_TIDE_LEVEL, SENSOR_MULTILEVEL_SCALE_METER, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_WEIGHT(GETTER)								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_WEIGHT, SENSOR_MULTILEVEL_SCALE_KILOGRAM, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_VOLTAGE(GETTER)								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_VOLTAGE, SENSOR_MULTILEVEL_SCALE_VOLT, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_CURRENT(GETTER)								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_CURRENT, SENSOR_MULTILEVEL_SCALE_AMPERE, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_CO2_LEVEL(GETTER)							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_CO2_LEVEL, SENSOR_MULTILEVEL_SCALE_PARTS_PER_MILLION, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_AIR_FLOW(GETTER)								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_AIR_FLOW, SENSOR_MULTILEVEL_SCALE_CUBIC_METER_PER_HOUR, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_TANK_CAPACITY(GETTER)						ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_TANK_CAPACITY, SENSOR_MULTILEVEL_SCALE_LITER, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_DISTANCE(GETTER)								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_DISTANCE, SENSOR_MULTILEVEL_SCALE_METER, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_MOISTURE(GETTER)								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_MOISTURE, SENSOR_MULTILEVEL_SCALE_PERCENTAGE_VALUE, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_ANGLE_POSITION(GETTER)						ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_ANGLE_POSITION, SENSOR_MULTILEVEL_SCALE_PERCENTAGE_VALUE, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_ROTATION(GETTER)								ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_ROTATION, SENSOR_MULTILEVEL_SCALE_REVOLUTION_PER_MINUTE, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_WATER_TEMPERATURE(GETTER)					ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_WATER_TEMPERATURE, SENSOR_MULTILEVEL_SCALE_CELSIUS, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_SOIL_TEMPERATURE(GETTER)						ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_SOIL_TEMPERATURE, SENSOR_MULTILEVEL_SCALE_CELSIUS, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_SEISMIC_INTENSITY(GETTER)					ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_SEISMIC_INTENSITY, SENSOR_MULTILEVEL_SCALE_MERCALLI, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_SEISMIC_MAGNITUDE(GETTER)					ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_SEISMIC_MAGNITUDE, SENSOR_MULTILEVEL_SCALE_LOCAL, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_ULTRAVIOLET(GETTER)							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_ULTRAVIOLET, SENSOR_MULTILEVEL_SCALE_ULTRAVIOLET_INDEX, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_ELECTRICAL_RESISTIVITY(GETTER)				ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_ELECTRICAL_RESISTIVITY, SENSOR_MULTILEVEL_SCALE_OHM_METER, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_LOUDNESS(GETTER)						ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_LOUDNESS, SENSOR_MULTILEVEL_SCALE_DECIBELS, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_ZERO_DECIMALS, GETTER))
#define ZUNO_SENSOR_MULTILEVEL_ELECTRICAL_CONDUCTIVITY(GETTER)				ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_ELECTRICAL_CONDUCTIVITY, SENSOR_MULTILEVEL_SCALE_SIEMENS_PER_METER, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_FREQUENCY(GETTER)							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_FREQUENCY, SENSOR_MULTILEVEL_SCALE_HERTZ, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_TIME(GETTER)									ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_TIME, SENSOR_MULTILEVEL_SCALE_SECOND, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_TARGET_TEMPERATURE(GETTER)					ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_TARGET_TEMPERATURE, SENSOR_MULTILEVEL_SCALE_CELSIUS, SENSOR_MULTILEVEL_SIZE_ONE_BYTE, SENSOR_MULTILEVEL_PRECISION_ZERO_DECIMALS, GETTER)
#define ZUNO_SENSOR_MULTILEVEL_WATER_FLOW(GETTER)							ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_WATER_FLOW, SENSOR_MULTILEVEL_SCALE_LITERS_PER_HOUR, SENSOR_MULTILEVEL_SIZE_TWO_BYTE, SENSOR_MULTILEVEL_PRECISION_ONE_DECIMAL, GETTER)

#define ZUNO_NO_ASSOCIATIONS							{0, 0}
#define ZUNO_ASSOCIATION_GROUP_SET_VALUE 				{ZUNO_ASSOC_BASIC_SET_NUMBER, ZUNO_ASSOC_NO_PARAMS}
#define ZUNO_ASSOCIATION_GROUP_SET_VALUE_AND_DIM 		{ZUNO_ASSOC_BASIC_SET_AND_DIM_NUMBER, ZUNO_ASSOC_NO_PARAMS}
#define ZUNO_ASSOCIATION_GROUP_SCENE_CONTROL 			{ZUNO_ASSOC_SCENE_ACTIVATION_NUMBER, ZUNO_ASSOC_NO_PARAMS}
#define ZUNO_ASSOCIATION_GROUP_DOORLOCK					{ZUNO_ASSOC_DOORLOCK_CONTROL_NUMBER, ZUNO_ASSOC_NO_PARAMS}

#define ZUNO_SLEEPING_MODE_ALWAYS_AWAKE 			    ZUNO_MODE_ALWAYS_LISTENING_NUMBER
#define ZUNO_SLEEPING_MODE_SLEEPING			 		    ZUNO_MODE_WAKE_UP_NUMBER
#define ZUNO_SLEEPING_MODE_FREQUENTLY_AWAKE 		    ZUNO_MODE_FLIRS_NUMBER

#endif
// -----------------------------------------------------------------
#ifdef ZUNO_STATICGATHERING_PHASE
#define ZUNO_SETUP_CHANNELS(...) 	\
							    ZUNO_CHANNEL_PROPERTIES_DESCRIPTION __zunoChannelSetupArray[]= \
								{ \
									__VA_ARGS__, \
								}
#define ZUNO_SETUP_ASSOCIATIONS(...)	\
								 ZUNO_ASSOCIATION_PROPERTIES_DESCRIPTION ___zunoAssociationSetupArray[]= \
								{ \
									__VA_ARGS__, \
								}
#define ZUNO_SETUP_SLEEPING_MODE(VALUE) 		\
								BYTE ___zunoSleepingModeValue = VALUE
#define ZUNO_SETUP_BATTERY_LEVELS(L,H) 		\
								word ___zunoBatteryLow = L; \
								word ___zunoBatteryHigh = H 
#define ZUNO_SETUP_CFGPARAMETER_HANDLER(H) 		\
								GENERIC_POINTER ___zunoCFGHandler = ((void*)H)
#define ZUNO_SETUP_BATTERY_HANDLER(H) 		\
								GENERIC_POINTER ___zunoBatteryHandler = ((void*)H)
#define ZUNO_REPORTS_HANDLER(N, H) 		\
								GENERIC_POINTER ___zunoReportsHandler_##N = ((void*)H)
#define ZUNO_SETUP_SYSEVENT_HANDLER(H)\
								GENERIC_POINTER ___zunoEventHandler = ((void*)H)
#define ZUNO_SETUP_SYSTIMER_HANDLER(H)\
								GENERIC_POINTER ___zunoSysTimerHandler = ((void*)H)
#define ZUNO_SETUP_PRODUCT_AUTO() \
								BYTE ___zunoProductType = 1
#define ZUNO_SETUP_PRODUCT_ID(MAJ,MIN) \
								BYTE ___zunoProductType = 2;\
								BYTE ___zunoProductIDMaj = MAJ;\
								BYTE ___zunoProductIDMin = MIN
#define ZUNO_SETUP_S2ACCESS(B) \
								BYTE ___zunoS2Access = B
#define ZUNO_DISABLE(VAR_NAMES)\
								char ___zunoDisableList[] = #VAR_NAMES
#define ZUNO_ENABLE(VAR_NAMES)\
								char ___zunoEnableList[] = #VAR_NAMES
#else
#define ZUNO_SETUP_CHANNELS(...)
#define ZUNO_SETUP_ASSOCIATIONS(...)
#define ZUNO_SETUP_SLEEPING_MODE(VALUE)  
#define ZUNO_SETUP_BATTERY_LEVELS(L,H)
#define ZUNO_SETUP_CFGPARAMETER_HANDLER(H)  
#define ZUNO_REPORTS_HANDLER(N, H)    
#define ZUNO_SETUP_SYSEVENT_HANDLER(H)
#define ZUNO_SETUP_SYSTIMER_HANDLER(H) 
#define ZUNO_SETUP_PRODUCT_AUTO()  
#define ZUNO_SETUP_PRODUCT_ID(MAJ,MIN)   
#define ZUNO_SETUP_S2ACCESS(B)   
#define ZUNO_DISABLE(VAR_NAMES)
#define ZUNO_ENABLE(VAR_NAMES)          
#endif
#endif // ZUNO_STATIC_DATA_H