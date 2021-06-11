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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/master_demo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/master_demo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../i2c_lib/i2c_master.S i2c_master_eeprom_24fc256.c master_demo_main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1456076783/i2c_master.o ${OBJECTDIR}/i2c_master_eeprom_24fc256.o ${OBJECTDIR}/master_demo_main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1456076783/i2c_master.o.d ${OBJECTDIR}/i2c_master_eeprom_24fc256.o.d ${OBJECTDIR}/master_demo_main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1456076783/i2c_master.o ${OBJECTDIR}/i2c_master_eeprom_24fc256.o ${OBJECTDIR}/master_demo_main.o

# Source Files
SOURCEFILES=../i2c_lib/i2c_master.S i2c_master_eeprom_24fc256.c master_demo_main.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/master_demo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MM0064GPL036
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1456076783/i2c_master.o: ../i2c_lib/i2c_master.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1456076783" 
	@${RM} ${OBJECTDIR}/_ext/1456076783/i2c_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/1456076783/i2c_master.o 
	@${RM} ${OBJECTDIR}/_ext/1456076783/i2c_master.o.ok ${OBJECTDIR}/_ext/1456076783/i2c_master.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1456076783/i2c_master.o.d" "${OBJECTDIR}/_ext/1456076783/i2c_master.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1456076783/i2c_master.o.d"  -o ${OBJECTDIR}/_ext/1456076783/i2c_master.o ../i2c_lib/i2c_master.S  -DXPRJ_CFGNAME=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1456076783/i2c_master.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1
	
else
${OBJECTDIR}/_ext/1456076783/i2c_master.o: ../i2c_lib/i2c_master.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1456076783" 
	@${RM} ${OBJECTDIR}/_ext/1456076783/i2c_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/1456076783/i2c_master.o 
	@${RM} ${OBJECTDIR}/_ext/1456076783/i2c_master.o.ok ${OBJECTDIR}/_ext/1456076783/i2c_master.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1456076783/i2c_master.o.d" "${OBJECTDIR}/_ext/1456076783/i2c_master.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1456076783/i2c_master.o.d"  -o ${OBJECTDIR}/_ext/1456076783/i2c_master.o ../i2c_lib/i2c_master.S  -DXPRJ_CFGNAME=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1456076783/i2c_master.o.asm.d",--gdwarf-2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/i2c_master_eeprom_24fc256.o: i2c_master_eeprom_24fc256.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c_master_eeprom_24fc256.o.d 
	@${RM} ${OBJECTDIR}/i2c_master_eeprom_24fc256.o 
	@${FIXDEPS} "${OBJECTDIR}/i2c_master_eeprom_24fc256.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mmicromips -Os -I"../i2c_lib" -MMD -MF "${OBJECTDIR}/i2c_master_eeprom_24fc256.o.d" -o ${OBJECTDIR}/i2c_master_eeprom_24fc256.o i2c_master_eeprom_24fc256.c    -DXPRJ_CFGNAME=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/master_demo_main.o: master_demo_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/master_demo_main.o.d 
	@${RM} ${OBJECTDIR}/master_demo_main.o 
	@${FIXDEPS} "${OBJECTDIR}/master_demo_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mmicromips -Os -I"../i2c_lib" -MMD -MF "${OBJECTDIR}/master_demo_main.o.d" -o ${OBJECTDIR}/master_demo_main.o master_demo_main.c    -DXPRJ_CFGNAME=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/i2c_master_eeprom_24fc256.o: i2c_master_eeprom_24fc256.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c_master_eeprom_24fc256.o.d 
	@${RM} ${OBJECTDIR}/i2c_master_eeprom_24fc256.o 
	@${FIXDEPS} "${OBJECTDIR}/i2c_master_eeprom_24fc256.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mmicromips -Os -I"../i2c_lib" -MMD -MF "${OBJECTDIR}/i2c_master_eeprom_24fc256.o.d" -o ${OBJECTDIR}/i2c_master_eeprom_24fc256.o i2c_master_eeprom_24fc256.c    -DXPRJ_CFGNAME=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/master_demo_main.o: master_demo_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/master_demo_main.o.d 
	@${RM} ${OBJECTDIR}/master_demo_main.o 
	@${FIXDEPS} "${OBJECTDIR}/master_demo_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mmicromips -Os -I"../i2c_lib" -MMD -MF "${OBJECTDIR}/master_demo_main.o.d" -o ${OBJECTDIR}/master_demo_main.o master_demo_main.c    -DXPRJ_CFGNAME=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/master_demo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_REAL_ICE=1 -mprocessor=$(MP_PROCESSOR_OPTION) -Os -mmicromips -o dist/${CND_CONF}/${IMAGE_TYPE}/master_demo.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_CFGNAME=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)    -mreserve=boot@0x1FC00490:0x1FC016FF -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/master_demo.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -Os -mmicromips -o dist/${CND_CONF}/${IMAGE_TYPE}/master_demo.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_CFGNAME=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/master_demo.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
