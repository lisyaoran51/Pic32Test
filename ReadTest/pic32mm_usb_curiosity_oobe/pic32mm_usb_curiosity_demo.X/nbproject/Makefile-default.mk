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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic32mm_usb_curiosity_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pic32mm_usb_curiosity_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=bsp/adc.c bsp/buttons.c bsp/leds.c bsp/leds_rgb.c bsp/timer_1ms.c usb/usb_device.c usb/usb_device_generic.c usb/usb_hal_32bit.c usb/usb_descriptors.c usb/usb_events.c main.c system.c app_device_vendor_basic.c app_led_usb_status.c uart1.c drivers/uart.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/bsp/adc.o ${OBJECTDIR}/bsp/buttons.o ${OBJECTDIR}/bsp/leds.o ${OBJECTDIR}/bsp/leds_rgb.o ${OBJECTDIR}/bsp/timer_1ms.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_generic.o ${OBJECTDIR}/usb/usb_hal_32bit.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system.o ${OBJECTDIR}/app_device_vendor_basic.o ${OBJECTDIR}/app_led_usb_status.o ${OBJECTDIR}/uart1.o ${OBJECTDIR}/drivers/uart.o
POSSIBLE_DEPFILES=${OBJECTDIR}/bsp/adc.o.d ${OBJECTDIR}/bsp/buttons.o.d ${OBJECTDIR}/bsp/leds.o.d ${OBJECTDIR}/bsp/leds_rgb.o.d ${OBJECTDIR}/bsp/timer_1ms.o.d ${OBJECTDIR}/usb/usb_device.o.d ${OBJECTDIR}/usb/usb_device_generic.o.d ${OBJECTDIR}/usb/usb_hal_32bit.o.d ${OBJECTDIR}/usb/usb_descriptors.o.d ${OBJECTDIR}/usb/usb_events.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/system.o.d ${OBJECTDIR}/app_device_vendor_basic.o.d ${OBJECTDIR}/app_led_usb_status.o.d ${OBJECTDIR}/uart1.o.d ${OBJECTDIR}/drivers/uart.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/bsp/adc.o ${OBJECTDIR}/bsp/buttons.o ${OBJECTDIR}/bsp/leds.o ${OBJECTDIR}/bsp/leds_rgb.o ${OBJECTDIR}/bsp/timer_1ms.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_generic.o ${OBJECTDIR}/usb/usb_hal_32bit.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system.o ${OBJECTDIR}/app_device_vendor_basic.o ${OBJECTDIR}/app_led_usb_status.o ${OBJECTDIR}/uart1.o ${OBJECTDIR}/drivers/uart.o

# Source Files
SOURCEFILES=bsp/adc.c bsp/buttons.c bsp/leds.c bsp/leds_rgb.c bsp/timer_1ms.c usb/usb_device.c usb/usb_device_generic.c usb/usb_hal_32bit.c usb/usb_descriptors.c usb/usb_events.c main.c system.c app_device_vendor_basic.c app_led_usb_status.c uart1.c drivers/uart.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/pic32mm_usb_curiosity_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MM0256GPM064
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/bsp/adc.o: bsp/adc.c  .generated_files/flags/default/1a465ad0f58f98e800b22762ac0ab1c5201a2cca .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/adc.o.d 
	@${RM} ${OBJECTDIR}/bsp/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/adc.o.d" -o ${OBJECTDIR}/bsp/adc.o bsp/adc.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/buttons.o: bsp/buttons.c  .generated_files/flags/default/bc7aa1182c87a7836be978b613083b37db126ad9 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/buttons.o.d 
	@${RM} ${OBJECTDIR}/bsp/buttons.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/buttons.o.d" -o ${OBJECTDIR}/bsp/buttons.o bsp/buttons.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/leds.o: bsp/leds.c  .generated_files/flags/default/68b6c226e070175fe8db2cbbf9ea3e706c54edbd .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/leds.o.d 
	@${RM} ${OBJECTDIR}/bsp/leds.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/leds.o.d" -o ${OBJECTDIR}/bsp/leds.o bsp/leds.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/leds_rgb.o: bsp/leds_rgb.c  .generated_files/flags/default/72c6ba63f21ad9a104a695ad07cb88081381d8a4 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/leds_rgb.o.d 
	@${RM} ${OBJECTDIR}/bsp/leds_rgb.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/leds_rgb.o.d" -o ${OBJECTDIR}/bsp/leds_rgb.o bsp/leds_rgb.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/timer_1ms.o: bsp/timer_1ms.c  .generated_files/flags/default/2ee0838004ccc0931d3e20eff5a6ffe1b311fca4 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/timer_1ms.o.d 
	@${RM} ${OBJECTDIR}/bsp/timer_1ms.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/timer_1ms.o.d" -o ${OBJECTDIR}/bsp/timer_1ms.o bsp/timer_1ms.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/59d53ea699b52c0a33e36e31efedb0cd7746118 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d" -o ${OBJECTDIR}/usb/usb_device.o usb/usb_device.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_device_generic.o: usb/usb_device_generic.c  .generated_files/flags/default/647df1ee545a499dc783bea1306d74790102fc19 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_generic.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_generic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_generic.o.d" -o ${OBJECTDIR}/usb/usb_device_generic.o usb/usb_device_generic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_hal_32bit.o: usb/usb_hal_32bit.c  .generated_files/flags/default/192c94d65dd1403c485bc586d18920e256028c93 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_32bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_32bit.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_32bit.o.d" -o ${OBJECTDIR}/usb/usb_hal_32bit.o usb/usb_hal_32bit.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/e42a105f257af2d6b33e2bbe5f14d047af27f96 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d" -o ${OBJECTDIR}/usb/usb_descriptors.o usb/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/31392975c24ddd8333d478b68f80c2597944767e .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d" -o ${OBJECTDIR}/usb/usb_events.o usb/usb_events.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/a1ea72b0c51babb649a10087fa6b263d1e50fc74 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/system.o: system.c  .generated_files/flags/default/a6a7534d1d10516b4a24ce6f846ef31e92712420 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/system.o.d" -o ${OBJECTDIR}/system.o system.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/app_device_vendor_basic.o: app_device_vendor_basic.c  .generated_files/flags/default/409baeacdff3723a2fbae9e98cf1a8286a6aa97c .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_device_vendor_basic.o.d 
	@${RM} ${OBJECTDIR}/app_device_vendor_basic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/app_device_vendor_basic.o.d" -o ${OBJECTDIR}/app_device_vendor_basic.o app_device_vendor_basic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/app_led_usb_status.o: app_led_usb_status.c  .generated_files/flags/default/12dd2ab12cade7592752e55950b3c250cee9582c .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_led_usb_status.o.d 
	@${RM} ${OBJECTDIR}/app_led_usb_status.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/app_led_usb_status.o.d" -o ${OBJECTDIR}/app_led_usb_status.o app_led_usb_status.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/uart1.o: uart1.c  .generated_files/flags/default/f98b50e718ebc5cbe65bcee5c9ca9de502cdd961 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart1.o.d 
	@${RM} ${OBJECTDIR}/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/uart1.o.d" -o ${OBJECTDIR}/uart1.o uart1.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/drivers/uart.o: drivers/uart.c  .generated_files/flags/default/d75f30050fbf2ede3865bdd66e756335fc87e1c1 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/drivers" 
	@${RM} ${OBJECTDIR}/drivers/uart.o.d 
	@${RM} ${OBJECTDIR}/drivers/uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/drivers/uart.o.d" -o ${OBJECTDIR}/drivers/uart.o drivers/uart.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/bsp/adc.o: bsp/adc.c  .generated_files/flags/default/f5857369c2956de360ead2a3e35fe213bff2872 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/adc.o.d 
	@${RM} ${OBJECTDIR}/bsp/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/adc.o.d" -o ${OBJECTDIR}/bsp/adc.o bsp/adc.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/buttons.o: bsp/buttons.c  .generated_files/flags/default/1c043e1d2816a66cfb162fad23c1eaafdf3b831e .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/buttons.o.d 
	@${RM} ${OBJECTDIR}/bsp/buttons.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/buttons.o.d" -o ${OBJECTDIR}/bsp/buttons.o bsp/buttons.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/leds.o: bsp/leds.c  .generated_files/flags/default/f3962f7f5d7ad44c00f2f0b045467ba4935ab115 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/leds.o.d 
	@${RM} ${OBJECTDIR}/bsp/leds.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/leds.o.d" -o ${OBJECTDIR}/bsp/leds.o bsp/leds.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/leds_rgb.o: bsp/leds_rgb.c  .generated_files/flags/default/f9405e27b926092b233c8be31c0a33d90b3bf035 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/leds_rgb.o.d 
	@${RM} ${OBJECTDIR}/bsp/leds_rgb.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/leds_rgb.o.d" -o ${OBJECTDIR}/bsp/leds_rgb.o bsp/leds_rgb.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/timer_1ms.o: bsp/timer_1ms.c  .generated_files/flags/default/2c582ef178180bdc33182ad5fc0c7216d4f575bc .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/timer_1ms.o.d 
	@${RM} ${OBJECTDIR}/bsp/timer_1ms.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/timer_1ms.o.d" -o ${OBJECTDIR}/bsp/timer_1ms.o bsp/timer_1ms.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/538c911ee7417dd7dd6c97cca880b40dd74a4704 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d" -o ${OBJECTDIR}/usb/usb_device.o usb/usb_device.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_device_generic.o: usb/usb_device_generic.c  .generated_files/flags/default/ee0c5f4f0ca614e6cb6484fc84f55a4d63657f13 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_generic.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_generic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_generic.o.d" -o ${OBJECTDIR}/usb/usb_device_generic.o usb/usb_device_generic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_hal_32bit.o: usb/usb_hal_32bit.c  .generated_files/flags/default/d0b051f177a9beb190834f1f6d815eecf34b0ba1 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_32bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_32bit.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_32bit.o.d" -o ${OBJECTDIR}/usb/usb_hal_32bit.o usb/usb_hal_32bit.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/191d9b08919c84b239237f250ee5e5d201528f5d .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d" -o ${OBJECTDIR}/usb/usb_descriptors.o usb/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/e409512c66d9ead72f8bc1c02c719683c9cf0fcb .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d" -o ${OBJECTDIR}/usb/usb_events.o usb/usb_events.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/d6bba57f953fccca33ea5be178b75fd11a365d8b .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/system.o: system.c  .generated_files/flags/default/ee11454ac23c42462d038ec060a8fcb4c1ebef69 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/system.o.d" -o ${OBJECTDIR}/system.o system.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/app_device_vendor_basic.o: app_device_vendor_basic.c  .generated_files/flags/default/c7a00a1a5d14ad5e120957f8f53e1793b981dce5 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_device_vendor_basic.o.d 
	@${RM} ${OBJECTDIR}/app_device_vendor_basic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/app_device_vendor_basic.o.d" -o ${OBJECTDIR}/app_device_vendor_basic.o app_device_vendor_basic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/app_led_usb_status.o: app_led_usb_status.c  .generated_files/flags/default/ee604fcfb55ac529d8a526e0fce3b85e732301e4 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_led_usb_status.o.d 
	@${RM} ${OBJECTDIR}/app_led_usb_status.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/app_led_usb_status.o.d" -o ${OBJECTDIR}/app_led_usb_status.o app_led_usb_status.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/uart1.o: uart1.c  .generated_files/flags/default/ebdd0727e47bee9a07658ab59c1626f83ec5e453 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart1.o.d 
	@${RM} ${OBJECTDIR}/uart1.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/uart1.o.d" -o ${OBJECTDIR}/uart1.o uart1.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/drivers/uart.o: drivers/uart.c  .generated_files/flags/default/c1091fd70be1db20f1f50cdad6658447e93be5a5 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/drivers" 
	@${RM} ${OBJECTDIR}/drivers/uart.o.d 
	@${RM} ${OBJECTDIR}/drivers/uart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/drivers/uart.o.d" -o ${OBJECTDIR}/drivers/uart.o drivers/uart.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/pic32mm_usb_curiosity_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic32mm_usb_curiosity_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/pic32mm_usb_curiosity_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pic32mm_usb_curiosity_demo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/pic32mm_usb_curiosity_demo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
