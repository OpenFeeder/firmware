#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-PIC24FJ128GB204_CodeModel_Large.mk)" "nbproject/Makefile-local-PIC24FJ128GB204_CodeModel_Large.mk"
include nbproject/Makefile-local-PIC24FJ128GB204_CodeModel_Large.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC24FJ128GB204_CodeModel_Large
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/mplab.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/mplab.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=framework/fileio/fileio.c framework/usb/usb_hal_pic24.c framework/usb/usb_host.c framework/usb/usb_host_msd.c framework/usb/usb_host_msd_scsi.c framework/usb/usb_config.c mcc_generated_files/mcc.c mcc_generated_files/interrupt_manager.c mcc_generated_files/rtcc.c mcc_generated_files/pin_manager.c mcc_generated_files/uart1.c mcc_generated_files/tmr2.c mcc_generated_files/oc1.c mcc_generated_files/traps.c mcc_generated_files/ext_int.c mcc_generated_files/tmr3.c mcc_generated_files/oc2.c mcc_generated_files/oc3.c mcc_generated_files/oc4.c mcc_generated_files/i2c1.c mcc_generated_files/adc1.c mcc_generated_files/tmr4.c mcc_generated_files/oc5.c main.c src/app.c src/app_alarm.c src/app_attractive_leds.c src/app_check.c src/app_config.c src/app_data_logger.c src/app_datetime.c src/app_debug.c src/app_error.c src/app_event.c src/app_i2c.c src/app_power.c src/app_remote.c src/app_rfid.c src/app_timers_callback.c src/app_usb.c src/delay.c src/digits.c src/em4095.c src/ir_sensor.c src/led_status.c src/mcp23017.c src/min_glue.c src/min_ini.c src/servomotor.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/framework/fileio/fileio.o ${OBJECTDIR}/framework/usb/usb_hal_pic24.o ${OBJECTDIR}/framework/usb/usb_host.o ${OBJECTDIR}/framework/usb/usb_host_msd.o ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o ${OBJECTDIR}/framework/usb/usb_config.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/rtcc.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/tmr2.o ${OBJECTDIR}/mcc_generated_files/oc1.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/ext_int.o ${OBJECTDIR}/mcc_generated_files/tmr3.o ${OBJECTDIR}/mcc_generated_files/oc2.o ${OBJECTDIR}/mcc_generated_files/oc3.o ${OBJECTDIR}/mcc_generated_files/oc4.o ${OBJECTDIR}/mcc_generated_files/i2c1.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/tmr4.o ${OBJECTDIR}/mcc_generated_files/oc5.o ${OBJECTDIR}/main.o ${OBJECTDIR}/src/app.o ${OBJECTDIR}/src/app_alarm.o ${OBJECTDIR}/src/app_attractive_leds.o ${OBJECTDIR}/src/app_check.o ${OBJECTDIR}/src/app_config.o ${OBJECTDIR}/src/app_data_logger.o ${OBJECTDIR}/src/app_datetime.o ${OBJECTDIR}/src/app_debug.o ${OBJECTDIR}/src/app_error.o ${OBJECTDIR}/src/app_event.o ${OBJECTDIR}/src/app_i2c.o ${OBJECTDIR}/src/app_power.o ${OBJECTDIR}/src/app_remote.o ${OBJECTDIR}/src/app_rfid.o ${OBJECTDIR}/src/app_timers_callback.o ${OBJECTDIR}/src/app_usb.o ${OBJECTDIR}/src/delay.o ${OBJECTDIR}/src/digits.o ${OBJECTDIR}/src/em4095.o ${OBJECTDIR}/src/ir_sensor.o ${OBJECTDIR}/src/led_status.o ${OBJECTDIR}/src/mcp23017.o ${OBJECTDIR}/src/min_glue.o ${OBJECTDIR}/src/min_ini.o ${OBJECTDIR}/src/servomotor.o
POSSIBLE_DEPFILES=${OBJECTDIR}/framework/fileio/fileio.o.d ${OBJECTDIR}/framework/usb/usb_hal_pic24.o.d ${OBJECTDIR}/framework/usb/usb_host.o.d ${OBJECTDIR}/framework/usb/usb_host_msd.o.d ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o.d ${OBJECTDIR}/framework/usb/usb_config.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/rtcc.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/mcc_generated_files/tmr2.o.d ${OBJECTDIR}/mcc_generated_files/oc1.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/ext_int.o.d ${OBJECTDIR}/mcc_generated_files/tmr3.o.d ${OBJECTDIR}/mcc_generated_files/oc2.o.d ${OBJECTDIR}/mcc_generated_files/oc3.o.d ${OBJECTDIR}/mcc_generated_files/oc4.o.d ${OBJECTDIR}/mcc_generated_files/i2c1.o.d ${OBJECTDIR}/mcc_generated_files/adc1.o.d ${OBJECTDIR}/mcc_generated_files/tmr4.o.d ${OBJECTDIR}/mcc_generated_files/oc5.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/src/app.o.d ${OBJECTDIR}/src/app_alarm.o.d ${OBJECTDIR}/src/app_attractive_leds.o.d ${OBJECTDIR}/src/app_check.o.d ${OBJECTDIR}/src/app_config.o.d ${OBJECTDIR}/src/app_data_logger.o.d ${OBJECTDIR}/src/app_datetime.o.d ${OBJECTDIR}/src/app_debug.o.d ${OBJECTDIR}/src/app_error.o.d ${OBJECTDIR}/src/app_event.o.d ${OBJECTDIR}/src/app_i2c.o.d ${OBJECTDIR}/src/app_power.o.d ${OBJECTDIR}/src/app_remote.o.d ${OBJECTDIR}/src/app_rfid.o.d ${OBJECTDIR}/src/app_timers_callback.o.d ${OBJECTDIR}/src/app_usb.o.d ${OBJECTDIR}/src/delay.o.d ${OBJECTDIR}/src/digits.o.d ${OBJECTDIR}/src/em4095.o.d ${OBJECTDIR}/src/ir_sensor.o.d ${OBJECTDIR}/src/led_status.o.d ${OBJECTDIR}/src/mcp23017.o.d ${OBJECTDIR}/src/min_glue.o.d ${OBJECTDIR}/src/min_ini.o.d ${OBJECTDIR}/src/servomotor.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/framework/fileio/fileio.o ${OBJECTDIR}/framework/usb/usb_hal_pic24.o ${OBJECTDIR}/framework/usb/usb_host.o ${OBJECTDIR}/framework/usb/usb_host_msd.o ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o ${OBJECTDIR}/framework/usb/usb_config.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/rtcc.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/tmr2.o ${OBJECTDIR}/mcc_generated_files/oc1.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/ext_int.o ${OBJECTDIR}/mcc_generated_files/tmr3.o ${OBJECTDIR}/mcc_generated_files/oc2.o ${OBJECTDIR}/mcc_generated_files/oc3.o ${OBJECTDIR}/mcc_generated_files/oc4.o ${OBJECTDIR}/mcc_generated_files/i2c1.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/tmr4.o ${OBJECTDIR}/mcc_generated_files/oc5.o ${OBJECTDIR}/main.o ${OBJECTDIR}/src/app.o ${OBJECTDIR}/src/app_alarm.o ${OBJECTDIR}/src/app_attractive_leds.o ${OBJECTDIR}/src/app_check.o ${OBJECTDIR}/src/app_config.o ${OBJECTDIR}/src/app_data_logger.o ${OBJECTDIR}/src/app_datetime.o ${OBJECTDIR}/src/app_debug.o ${OBJECTDIR}/src/app_error.o ${OBJECTDIR}/src/app_event.o ${OBJECTDIR}/src/app_i2c.o ${OBJECTDIR}/src/app_power.o ${OBJECTDIR}/src/app_remote.o ${OBJECTDIR}/src/app_rfid.o ${OBJECTDIR}/src/app_timers_callback.o ${OBJECTDIR}/src/app_usb.o ${OBJECTDIR}/src/delay.o ${OBJECTDIR}/src/digits.o ${OBJECTDIR}/src/em4095.o ${OBJECTDIR}/src/ir_sensor.o ${OBJECTDIR}/src/led_status.o ${OBJECTDIR}/src/mcp23017.o ${OBJECTDIR}/src/min_glue.o ${OBJECTDIR}/src/min_ini.o ${OBJECTDIR}/src/servomotor.o

# Source Files
SOURCEFILES=framework/fileio/fileio.c framework/usb/usb_hal_pic24.c framework/usb/usb_host.c framework/usb/usb_host_msd.c framework/usb/usb_host_msd_scsi.c framework/usb/usb_config.c mcc_generated_files/mcc.c mcc_generated_files/interrupt_manager.c mcc_generated_files/rtcc.c mcc_generated_files/pin_manager.c mcc_generated_files/uart1.c mcc_generated_files/tmr2.c mcc_generated_files/oc1.c mcc_generated_files/traps.c mcc_generated_files/ext_int.c mcc_generated_files/tmr3.c mcc_generated_files/oc2.c mcc_generated_files/oc3.c mcc_generated_files/oc4.c mcc_generated_files/i2c1.c mcc_generated_files/adc1.c mcc_generated_files/tmr4.c mcc_generated_files/oc5.c main.c src/app.c src/app_alarm.c src/app_attractive_leds.c src/app_check.c src/app_config.c src/app_data_logger.c src/app_datetime.c src/app_debug.c src/app_error.c src/app_event.c src/app_i2c.c src/app_power.c src/app_remote.c src/app_rfid.c src/app_timers_callback.c src/app_usb.c src/delay.c src/digits.c src/em4095.c src/ir_sensor.c src/led_status.c src/mcp23017.c src/min_glue.c src/min_ini.c src/servomotor.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-PIC24FJ128GB204_CodeModel_Large.mk dist/${CND_CONF}/${IMAGE_TYPE}/mplab.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ128GB204
MP_LINKER_FILE_OPTION=,--script=p24FJ128GB204.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/framework/fileio/fileio.o: framework/fileio/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/fileio" 
	@${RM} ${OBJECTDIR}/framework/fileio/fileio.o.d 
	@${RM} ${OBJECTDIR}/framework/fileio/fileio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/fileio/fileio.c  -o ${OBJECTDIR}/framework/fileio/fileio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/fileio/fileio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/fileio/fileio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_hal_pic24.o: framework/usb/usb_hal_pic24.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_hal_pic24.c  -o ${OBJECTDIR}/framework/usb/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_hal_pic24.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_hal_pic24.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_host.o: framework/usb/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_host.c  -o ${OBJECTDIR}/framework/usb/usb_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_host.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_host.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_host_msd.o: framework/usb/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host_msd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_host_msd.c  -o ${OBJECTDIR}/framework/usb/usb_host_msd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_host_msd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_host_msd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o: framework/usb/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_host_msd_scsi.c  -o ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_config.o: framework/usb/usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_config.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_config.c  -o ${OBJECTDIR}/framework/usb/usb_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/mcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/rtcc.c  -o ${OBJECTDIR}/mcc_generated_files/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/rtcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr2.o: mcc_generated_files/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr2.c  -o ${OBJECTDIR}/mcc_generated_files/tmr2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc1.o: mcc_generated_files/oc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc1.c  -o ${OBJECTDIR}/mcc_generated_files/oc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/ext_int.o: mcc_generated_files/ext_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/ext_int.c  -o ${OBJECTDIR}/mcc_generated_files/ext_int.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/ext_int.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/ext_int.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr3.o: mcc_generated_files/tmr3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr3.c  -o ${OBJECTDIR}/mcc_generated_files/tmr3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc2.o: mcc_generated_files/oc2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc2.c  -o ${OBJECTDIR}/mcc_generated_files/oc2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc3.o: mcc_generated_files/oc3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc3.c  -o ${OBJECTDIR}/mcc_generated_files/oc3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc4.o: mcc_generated_files/oc4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc4.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc4.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc4.c  -o ${OBJECTDIR}/mcc_generated_files/oc4.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc4.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc4.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/i2c1.o: mcc_generated_files/i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c1.c  -o ${OBJECTDIR}/mcc_generated_files/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/i2c1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/adc1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr4.o: mcc_generated_files/tmr4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr4.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr4.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr4.c  -o ${OBJECTDIR}/mcc_generated_files/tmr4.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr4.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr4.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc5.o: mcc_generated_files/oc5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc5.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc5.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc5.c  -o ${OBJECTDIR}/mcc_generated_files/oc5.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc5.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc5.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app.o: src/app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app.o.d 
	@${RM} ${OBJECTDIR}/src/app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app.c  -o ${OBJECTDIR}/src/app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_alarm.o: src/app_alarm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_alarm.o.d 
	@${RM} ${OBJECTDIR}/src/app_alarm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_alarm.c  -o ${OBJECTDIR}/src/app_alarm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_alarm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_alarm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_attractive_leds.o: src/app_attractive_leds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_attractive_leds.o.d 
	@${RM} ${OBJECTDIR}/src/app_attractive_leds.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_attractive_leds.c  -o ${OBJECTDIR}/src/app_attractive_leds.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_attractive_leds.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_attractive_leds.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_check.o: src/app_check.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_check.o.d 
	@${RM} ${OBJECTDIR}/src/app_check.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_check.c  -o ${OBJECTDIR}/src/app_check.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_check.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_check.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_config.o: src/app_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_config.o.d 
	@${RM} ${OBJECTDIR}/src/app_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_config.c  -o ${OBJECTDIR}/src/app_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_config.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_data_logger.o: src/app_data_logger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_data_logger.o.d 
	@${RM} ${OBJECTDIR}/src/app_data_logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_data_logger.c  -o ${OBJECTDIR}/src/app_data_logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_data_logger.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_data_logger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_datetime.o: src/app_datetime.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_datetime.o.d 
	@${RM} ${OBJECTDIR}/src/app_datetime.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_datetime.c  -o ${OBJECTDIR}/src/app_datetime.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_datetime.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_datetime.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_debug.o: src/app_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_debug.o.d 
	@${RM} ${OBJECTDIR}/src/app_debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_debug.c  -o ${OBJECTDIR}/src/app_debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_debug.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_error.o: src/app_error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_error.o.d 
	@${RM} ${OBJECTDIR}/src/app_error.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_error.c  -o ${OBJECTDIR}/src/app_error.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_error.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_error.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_event.o: src/app_event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_event.o.d 
	@${RM} ${OBJECTDIR}/src/app_event.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_event.c  -o ${OBJECTDIR}/src/app_event.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_event.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_event.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_i2c.o: src/app_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_i2c.o.d 
	@${RM} ${OBJECTDIR}/src/app_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_i2c.c  -o ${OBJECTDIR}/src/app_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_power.o: src/app_power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_power.o.d 
	@${RM} ${OBJECTDIR}/src/app_power.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_power.c  -o ${OBJECTDIR}/src/app_power.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_power.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_power.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_remote.o: src/app_remote.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_remote.o.d 
	@${RM} ${OBJECTDIR}/src/app_remote.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_remote.c  -o ${OBJECTDIR}/src/app_remote.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_remote.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_remote.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_rfid.o: src/app_rfid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_rfid.o.d 
	@${RM} ${OBJECTDIR}/src/app_rfid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_rfid.c  -o ${OBJECTDIR}/src/app_rfid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_rfid.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_rfid.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_timers_callback.o: src/app_timers_callback.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_timers_callback.o.d 
	@${RM} ${OBJECTDIR}/src/app_timers_callback.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_timers_callback.c  -o ${OBJECTDIR}/src/app_timers_callback.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_timers_callback.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_timers_callback.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_usb.o: src/app_usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_usb.o.d 
	@${RM} ${OBJECTDIR}/src/app_usb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_usb.c  -o ${OBJECTDIR}/src/app_usb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_usb.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_usb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/delay.o: src/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/delay.o.d 
	@${RM} ${OBJECTDIR}/src/delay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/delay.c  -o ${OBJECTDIR}/src/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/delay.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/delay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/digits.o: src/digits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/digits.o.d 
	@${RM} ${OBJECTDIR}/src/digits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/digits.c  -o ${OBJECTDIR}/src/digits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/digits.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/digits.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/em4095.o: src/em4095.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/em4095.o.d 
	@${RM} ${OBJECTDIR}/src/em4095.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/em4095.c  -o ${OBJECTDIR}/src/em4095.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/em4095.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/em4095.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/ir_sensor.o: src/ir_sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ir_sensor.o.d 
	@${RM} ${OBJECTDIR}/src/ir_sensor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/ir_sensor.c  -o ${OBJECTDIR}/src/ir_sensor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/ir_sensor.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/ir_sensor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/led_status.o: src/led_status.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/led_status.o.d 
	@${RM} ${OBJECTDIR}/src/led_status.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/led_status.c  -o ${OBJECTDIR}/src/led_status.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/led_status.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/led_status.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/mcp23017.o: src/mcp23017.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mcp23017.o.d 
	@${RM} ${OBJECTDIR}/src/mcp23017.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/mcp23017.c  -o ${OBJECTDIR}/src/mcp23017.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/mcp23017.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/mcp23017.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/min_glue.o: src/min_glue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/min_glue.o.d 
	@${RM} ${OBJECTDIR}/src/min_glue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/min_glue.c  -o ${OBJECTDIR}/src/min_glue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/min_glue.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/min_glue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/min_ini.o: src/min_ini.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/min_ini.o.d 
	@${RM} ${OBJECTDIR}/src/min_ini.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/min_ini.c  -o ${OBJECTDIR}/src/min_ini.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/min_ini.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/min_ini.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/servomotor.o: src/servomotor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/servomotor.o.d 
	@${RM} ${OBJECTDIR}/src/servomotor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/servomotor.c  -o ${OBJECTDIR}/src/servomotor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/servomotor.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/servomotor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/framework/fileio/fileio.o: framework/fileio/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/fileio" 
	@${RM} ${OBJECTDIR}/framework/fileio/fileio.o.d 
	@${RM} ${OBJECTDIR}/framework/fileio/fileio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/fileio/fileio.c  -o ${OBJECTDIR}/framework/fileio/fileio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/fileio/fileio.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/fileio/fileio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_hal_pic24.o: framework/usb/usb_hal_pic24.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_hal_pic24.c  -o ${OBJECTDIR}/framework/usb/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_hal_pic24.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_hal_pic24.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_host.o: framework/usb/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_host.c  -o ${OBJECTDIR}/framework/usb/usb_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_host.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_host.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_host_msd.o: framework/usb/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host_msd.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host_msd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_host_msd.c  -o ${OBJECTDIR}/framework/usb/usb_host_msd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_host_msd.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_host_msd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o: framework/usb/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_host_msd_scsi.c  -o ${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_host_msd_scsi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/framework/usb/usb_config.o: framework/usb/usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/framework/usb" 
	@${RM} ${OBJECTDIR}/framework/usb/usb_config.o.d 
	@${RM} ${OBJECTDIR}/framework/usb/usb_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  framework/usb/usb_config.c  -o ${OBJECTDIR}/framework/usb/usb_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/framework/usb/usb_config.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/framework/usb/usb_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/mcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/rtcc.c  -o ${OBJECTDIR}/mcc_generated_files/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/rtcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr2.o: mcc_generated_files/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr2.c  -o ${OBJECTDIR}/mcc_generated_files/tmr2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc1.o: mcc_generated_files/oc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc1.c  -o ${OBJECTDIR}/mcc_generated_files/oc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/ext_int.o: mcc_generated_files/ext_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/ext_int.c  -o ${OBJECTDIR}/mcc_generated_files/ext_int.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/ext_int.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/ext_int.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr3.o: mcc_generated_files/tmr3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr3.c  -o ${OBJECTDIR}/mcc_generated_files/tmr3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc2.o: mcc_generated_files/oc2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc2.c  -o ${OBJECTDIR}/mcc_generated_files/oc2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc3.o: mcc_generated_files/oc3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc3.c  -o ${OBJECTDIR}/mcc_generated_files/oc3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc4.o: mcc_generated_files/oc4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc4.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc4.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc4.c  -o ${OBJECTDIR}/mcc_generated_files/oc4.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc4.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc4.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/i2c1.o: mcc_generated_files/i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c1.c  -o ${OBJECTDIR}/mcc_generated_files/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/i2c1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/adc1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr4.o: mcc_generated_files/tmr4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr4.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr4.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr4.c  -o ${OBJECTDIR}/mcc_generated_files/tmr4.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr4.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr4.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/oc5.o: mcc_generated_files/oc5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc5.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/oc5.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/oc5.c  -o ${OBJECTDIR}/mcc_generated_files/oc5.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/oc5.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/oc5.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app.o: src/app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app.o.d 
	@${RM} ${OBJECTDIR}/src/app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app.c  -o ${OBJECTDIR}/src/app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_alarm.o: src/app_alarm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_alarm.o.d 
	@${RM} ${OBJECTDIR}/src/app_alarm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_alarm.c  -o ${OBJECTDIR}/src/app_alarm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_alarm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_alarm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_attractive_leds.o: src/app_attractive_leds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_attractive_leds.o.d 
	@${RM} ${OBJECTDIR}/src/app_attractive_leds.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_attractive_leds.c  -o ${OBJECTDIR}/src/app_attractive_leds.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_attractive_leds.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_attractive_leds.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_check.o: src/app_check.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_check.o.d 
	@${RM} ${OBJECTDIR}/src/app_check.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_check.c  -o ${OBJECTDIR}/src/app_check.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_check.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_check.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_config.o: src/app_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_config.o.d 
	@${RM} ${OBJECTDIR}/src/app_config.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_config.c  -o ${OBJECTDIR}/src/app_config.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_config.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_config.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_data_logger.o: src/app_data_logger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_data_logger.o.d 
	@${RM} ${OBJECTDIR}/src/app_data_logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_data_logger.c  -o ${OBJECTDIR}/src/app_data_logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_data_logger.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_data_logger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_datetime.o: src/app_datetime.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_datetime.o.d 
	@${RM} ${OBJECTDIR}/src/app_datetime.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_datetime.c  -o ${OBJECTDIR}/src/app_datetime.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_datetime.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_datetime.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_debug.o: src/app_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_debug.o.d 
	@${RM} ${OBJECTDIR}/src/app_debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_debug.c  -o ${OBJECTDIR}/src/app_debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_debug.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_error.o: src/app_error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_error.o.d 
	@${RM} ${OBJECTDIR}/src/app_error.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_error.c  -o ${OBJECTDIR}/src/app_error.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_error.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_error.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_event.o: src/app_event.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_event.o.d 
	@${RM} ${OBJECTDIR}/src/app_event.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_event.c  -o ${OBJECTDIR}/src/app_event.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_event.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_event.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_i2c.o: src/app_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_i2c.o.d 
	@${RM} ${OBJECTDIR}/src/app_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_i2c.c  -o ${OBJECTDIR}/src/app_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_i2c.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_power.o: src/app_power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_power.o.d 
	@${RM} ${OBJECTDIR}/src/app_power.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_power.c  -o ${OBJECTDIR}/src/app_power.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_power.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_power.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_remote.o: src/app_remote.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_remote.o.d 
	@${RM} ${OBJECTDIR}/src/app_remote.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_remote.c  -o ${OBJECTDIR}/src/app_remote.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_remote.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_remote.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_rfid.o: src/app_rfid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_rfid.o.d 
	@${RM} ${OBJECTDIR}/src/app_rfid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_rfid.c  -o ${OBJECTDIR}/src/app_rfid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_rfid.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_rfid.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_timers_callback.o: src/app_timers_callback.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_timers_callback.o.d 
	@${RM} ${OBJECTDIR}/src/app_timers_callback.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_timers_callback.c  -o ${OBJECTDIR}/src/app_timers_callback.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_timers_callback.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_timers_callback.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/app_usb.o: src/app_usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/app_usb.o.d 
	@${RM} ${OBJECTDIR}/src/app_usb.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/app_usb.c  -o ${OBJECTDIR}/src/app_usb.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/app_usb.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/app_usb.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/delay.o: src/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/delay.o.d 
	@${RM} ${OBJECTDIR}/src/delay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/delay.c  -o ${OBJECTDIR}/src/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/delay.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/delay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/digits.o: src/digits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/digits.o.d 
	@${RM} ${OBJECTDIR}/src/digits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/digits.c  -o ${OBJECTDIR}/src/digits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/digits.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/digits.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/em4095.o: src/em4095.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/em4095.o.d 
	@${RM} ${OBJECTDIR}/src/em4095.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/em4095.c  -o ${OBJECTDIR}/src/em4095.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/em4095.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/em4095.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/ir_sensor.o: src/ir_sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ir_sensor.o.d 
	@${RM} ${OBJECTDIR}/src/ir_sensor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/ir_sensor.c  -o ${OBJECTDIR}/src/ir_sensor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/ir_sensor.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/ir_sensor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/led_status.o: src/led_status.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/led_status.o.d 
	@${RM} ${OBJECTDIR}/src/led_status.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/led_status.c  -o ${OBJECTDIR}/src/led_status.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/led_status.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/led_status.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/mcp23017.o: src/mcp23017.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mcp23017.o.d 
	@${RM} ${OBJECTDIR}/src/mcp23017.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/mcp23017.c  -o ${OBJECTDIR}/src/mcp23017.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/mcp23017.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/mcp23017.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/min_glue.o: src/min_glue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/min_glue.o.d 
	@${RM} ${OBJECTDIR}/src/min_glue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/min_glue.c  -o ${OBJECTDIR}/src/min_glue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/min_glue.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/min_glue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/min_ini.o: src/min_ini.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/min_ini.o.d 
	@${RM} ${OBJECTDIR}/src/min_ini.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/min_ini.c  -o ${OBJECTDIR}/src/min_ini.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/min_ini.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/min_ini.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/servomotor.o: src/servomotor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/servomotor.o.d 
	@${RM} ${OBJECTDIR}/src/servomotor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/servomotor.c  -o ${OBJECTDIR}/src/servomotor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/servomotor.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"." -I"src" -I"framework/fileio" -I"framework/usb" -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/servomotor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/mplab.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/mplab.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=512,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/mplab.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/mplab.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_PIC24FJ128GB204_CodeModel_Large=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=512,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/mplab.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/PIC24FJ128GB204_CodeModel_Large
	${RM} -r dist/PIC24FJ128GB204_CodeModel_Large

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
