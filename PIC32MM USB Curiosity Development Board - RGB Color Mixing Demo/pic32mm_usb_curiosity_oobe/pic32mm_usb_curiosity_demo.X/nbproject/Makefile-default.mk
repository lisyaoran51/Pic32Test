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
SOURCEFILES_QUOTED_IF_SPACED=bsp/adc.c bsp/buttons.c bsp/leds.c bsp/leds_rgb.c bsp/timer_1ms.c usb/usb_device.c usb/usb_device_generic.c usb/usb_hal_32bit.c usb/usb_descriptors.c usb/usb_events.c main.c system.c app_device_vendor_basic.c app_led_usb_status.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/bsp/adc.o ${OBJECTDIR}/bsp/buttons.o ${OBJECTDIR}/bsp/leds.o ${OBJECTDIR}/bsp/leds_rgb.o ${OBJECTDIR}/bsp/timer_1ms.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_generic.o ${OBJECTDIR}/usb/usb_hal_32bit.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system.o ${OBJECTDIR}/app_device_vendor_basic.o ${OBJECTDIR}/app_led_usb_status.o
POSSIBLE_DEPFILES=${OBJECTDIR}/bsp/adc.o.d ${OBJECTDIR}/bsp/buttons.o.d ${OBJECTDIR}/bsp/leds.o.d ${OBJECTDIR}/bsp/leds_rgb.o.d ${OBJECTDIR}/bsp/timer_1ms.o.d ${OBJECTDIR}/usb/usb_device.o.d ${OBJECTDIR}/usb/usb_device_generic.o.d ${OBJECTDIR}/usb/usb_hal_32bit.o.d ${OBJECTDIR}/usb/usb_descriptors.o.d ${OBJECTDIR}/usb/usb_events.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/system.o.d ${OBJECTDIR}/app_device_vendor_basic.o.d ${OBJECTDIR}/app_led_usb_status.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/bsp/adc.o ${OBJECTDIR}/bsp/buttons.o ${OBJECTDIR}/bsp/leds.o ${OBJECTDIR}/bsp/leds_rgb.o ${OBJECTDIR}/bsp/timer_1ms.o ${OBJECTDIR}/usb/usb_device.o ${OBJECTDIR}/usb/usb_device_generic.o ${OBJECTDIR}/usb/usb_hal_32bit.o ${OBJECTDIR}/usb/usb_descriptors.o ${OBJECTDIR}/usb/usb_events.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system.o ${OBJECTDIR}/app_device_vendor_basic.o ${OBJECTDIR}/app_led_usb_status.o

# Source Files
SOURCEFILES=bsp/adc.c bsp/buttons.c bsp/leds.c bsp/leds_rgb.c bsp/timer_1ms.c usb/usb_device.c usb/usb_device_generic.c usb/usb_hal_32bit.c usb/usb_descriptors.c usb/usb_events.c main.c system.c app_device_vendor_basic.c app_led_usb_status.c



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
${OBJECTDIR}/bsp/adc.o: bsp/adc.c  .generated_files/flags/default/a8855a559e9a5a4a0721add8eeec8e1d36c23489 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/adc.o.d 
	@${RM} ${OBJECTDIR}/bsp/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/adc.o.d" -o ${OBJECTDIR}/bsp/adc.o bsp/adc.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/buttons.o: bsp/buttons.c  .generated_files/flags/default/e08a68c25debffe666fcf7359232a7a79abdbc89 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/buttons.o.d 
	@${RM} ${OBJECTDIR}/bsp/buttons.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/buttons.o.d" -o ${OBJECTDIR}/bsp/buttons.o bsp/buttons.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/leds.o: bsp/leds.c  .generated_files/flags/default/6ed41cbd1448876860f0bc3d4ac82300fa33bece .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/leds.o.d 
	@${RM} ${OBJECTDIR}/bsp/leds.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/leds.o.d" -o ${OBJECTDIR}/bsp/leds.o bsp/leds.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/leds_rgb.o: bsp/leds_rgb.c  .generated_files/flags/default/89709cda4f3f5a528a674fdfbd4a787e137e9103 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/leds_rgb.o.d 
	@${RM} ${OBJECTDIR}/bsp/leds_rgb.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/leds_rgb.o.d" -o ${OBJECTDIR}/bsp/leds_rgb.o bsp/leds_rgb.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/timer_1ms.o: bsp/timer_1ms.c  .generated_files/flags/default/3de9ee2794cc67bfbe7a181818b26cabe5a7aaa6 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/timer_1ms.o.d 
	@${RM} ${OBJECTDIR}/bsp/timer_1ms.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/timer_1ms.o.d" -o ${OBJECTDIR}/bsp/timer_1ms.o bsp/timer_1ms.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/fab138b7223bc08834f558932a68d0bd301e2fdb .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d" -o ${OBJECTDIR}/usb/usb_device.o usb/usb_device.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_device_generic.o: usb/usb_device_generic.c  .generated_files/flags/default/fc340b8b9d80d26d5fa531068cfbd80c081d167a .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_generic.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_generic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_generic.o.d" -o ${OBJECTDIR}/usb/usb_device_generic.o usb/usb_device_generic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_hal_32bit.o: usb/usb_hal_32bit.c  .generated_files/flags/default/340f4671af3680d2cc3bfb17b5d4ee382b101926 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_32bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_32bit.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_32bit.o.d" -o ${OBJECTDIR}/usb/usb_hal_32bit.o usb/usb_hal_32bit.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/a15e7d1f3b37ee5d33b6dcf85589ba96223963be .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d" -o ${OBJECTDIR}/usb/usb_descriptors.o usb/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/f300d069fdc9c29755708d57ad9ba916b36b375e .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d" -o ${OBJECTDIR}/usb/usb_events.o usb/usb_events.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/3fe2f8e5cd4ec84cd79210c517fc0d83dd656c59 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/system.o: system.c  .generated_files/flags/default/6c0c9f719b4a18de53a4b9086a5a18d3b6c47ff .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/system.o.d" -o ${OBJECTDIR}/system.o system.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/app_device_vendor_basic.o: app_device_vendor_basic.c  .generated_files/flags/default/80bbad7c4e0cb2d7cdea45ad0556678ff0abbfbe .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_device_vendor_basic.o.d 
	@${RM} ${OBJECTDIR}/app_device_vendor_basic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/app_device_vendor_basic.o.d" -o ${OBJECTDIR}/app_device_vendor_basic.o app_device_vendor_basic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/app_led_usb_status.o: app_led_usb_status.c  .generated_files/flags/default/ed4da03512fc32a0b66ac104f05aa9dce4cb437e .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_led_usb_status.o.d 
	@${RM} ${OBJECTDIR}/app_led_usb_status.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/app_led_usb_status.o.d" -o ${OBJECTDIR}/app_led_usb_status.o app_led_usb_status.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/bsp/adc.o: bsp/adc.c  .generated_files/flags/default/37736b7270ebb3d88b665660d75841125c7e4e83 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/adc.o.d 
	@${RM} ${OBJECTDIR}/bsp/adc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/adc.o.d" -o ${OBJECTDIR}/bsp/adc.o bsp/adc.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/buttons.o: bsp/buttons.c  .generated_files/flags/default/ee0b083b3a1d79ada767943e29f48e3d1e19980f .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/buttons.o.d 
	@${RM} ${OBJECTDIR}/bsp/buttons.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/buttons.o.d" -o ${OBJECTDIR}/bsp/buttons.o bsp/buttons.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/leds.o: bsp/leds.c  .generated_files/flags/default/e0f0c5dc3b382c686d57a9a65999a8216836348b .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/leds.o.d 
	@${RM} ${OBJECTDIR}/bsp/leds.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/leds.o.d" -o ${OBJECTDIR}/bsp/leds.o bsp/leds.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/leds_rgb.o: bsp/leds_rgb.c  .generated_files/flags/default/136dcc5ce8666ba37adb5a8f7416d01515578095 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/leds_rgb.o.d 
	@${RM} ${OBJECTDIR}/bsp/leds_rgb.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/leds_rgb.o.d" -o ${OBJECTDIR}/bsp/leds_rgb.o bsp/leds_rgb.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/bsp/timer_1ms.o: bsp/timer_1ms.c  .generated_files/flags/default/6f87bde83aa9925670667559216966b0b56561f3 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/bsp" 
	@${RM} ${OBJECTDIR}/bsp/timer_1ms.o.d 
	@${RM} ${OBJECTDIR}/bsp/timer_1ms.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/bsp/timer_1ms.o.d" -o ${OBJECTDIR}/bsp/timer_1ms.o bsp/timer_1ms.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_device.o: usb/usb_device.c  .generated_files/flags/default/7ca75bb0019877d197b56d2ef9dcaf4aaa1750b5 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_device.o.d" -o ${OBJECTDIR}/usb/usb_device.o usb/usb_device.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_device_generic.o: usb/usb_device_generic.c  .generated_files/flags/default/54b744a1c68919d9a36102df8623eb474fd3663e .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_device_generic.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_device_generic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_device_generic.o.d" -o ${OBJECTDIR}/usb/usb_device_generic.o usb/usb_device_generic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_hal_32bit.o: usb/usb_hal_32bit.c  .generated_files/flags/default/55353adaa8e95fa74b54ac749dc74d2038a9e652 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_hal_32bit.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_hal_32bit.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_hal_32bit.o.d" -o ${OBJECTDIR}/usb/usb_hal_32bit.o usb/usb_hal_32bit.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_descriptors.o: usb/usb_descriptors.c  .generated_files/flags/default/6776e438e143a17703e26c4810a777d964dc4bf1 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_descriptors.o.d" -o ${OBJECTDIR}/usb/usb_descriptors.o usb/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/usb/usb_events.o: usb/usb_events.c  .generated_files/flags/default/906347c9be246792c27e00325306652c483ac3a4 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}/usb" 
	@${RM} ${OBJECTDIR}/usb/usb_events.o.d 
	@${RM} ${OBJECTDIR}/usb/usb_events.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/usb/usb_events.o.d" -o ${OBJECTDIR}/usb/usb_events.o usb/usb_events.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/7df916b5e2d500c263e4bece3498f63f6a164768 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/system.o: system.c  .generated_files/flags/default/70c2d57676930667cf9a95115c1b0d5fac53d4d6 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/system.o.d" -o ${OBJECTDIR}/system.o system.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/app_device_vendor_basic.o: app_device_vendor_basic.c  .generated_files/flags/default/d404815f66478985de069686a924f18506c3e35d .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_device_vendor_basic.o.d 
	@${RM} ${OBJECTDIR}/app_device_vendor_basic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/app_device_vendor_basic.o.d" -o ${OBJECTDIR}/app_device_vendor_basic.o app_device_vendor_basic.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/app_led_usb_status.o: app_led_usb_status.c  .generated_files/flags/default/8535b3162ab2da8e2c52b4cf7f0636de402e1c70 .generated_files/flags/default/11b4d79b4b2276b8620d4f700419d4bd705a852a
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_led_usb_status.o.d 
	@${RM} ${OBJECTDIR}/app_led_usb_status.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DSYSTEM_PERIPHERAL_CLOCK=32000000 -I"bsp" -I"usb" -I".." -I"." -MP -MMD -MF "${OBJECTDIR}/app_led_usb_status.o.d" -o ${OBJECTDIR}/app_led_usb_status.o app_led_usb_status.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
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
