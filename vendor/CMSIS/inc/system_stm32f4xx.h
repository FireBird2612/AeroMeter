/**
 ******************************************************************************
 * @file    system_stm32f4xx.h
 * @author  MCD Application Team
 * @brief   CMSIS Cortex-M4 Device System Header File.
 ******************************************************************************
 */

#ifndef __SYSTEM_STM32F4xx_H
#define __SYSTEM_STM32F4xx_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @addtogroup CMSIS
 * @{
 */

/** @addtogroup stm32f4xx_system
 * @{
 */

/**
 * @brief Define the value of the External oscillator in Hz
 */
#if !defined(HSE_VALUE)
#define HSE_VALUE                                                              \
  ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
#endif                 /* HSE_VALUE */

/**
 * @brief Define the value of the Internal oscillator in Hz
 */
#if !defined(HSI_VALUE)
#define HSI_VALUE                                                              \
  ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz */
#endif                 /* HSI_VALUE */

/**
 * @brief Uncomment the line below according to the target STM32F4xx device used
 * in your application
 */
#if !defined(STM32F405xx) && !defined(STM32F415xx) && !defined(STM32F407xx) && \
    !defined(STM32F417xx) && !defined(STM32F427xx) && !defined(STM32F437xx) && \
    !defined(STM32F429xx) && !defined(STM32F439xx) && !defined(STM32F401xC) && \
    !defined(STM32F401xE) && !defined(STM32F410Tx) && !defined(STM32F410Cx) && \
    !defined(STM32F410Rx) && !defined(STM32F411xE) && !defined(STM32F446xx) && \
    !defined(STM32F469xx) && !defined(STM32F479xx) && !defined(STM32F412Cx) && \
    !defined(STM32F412Rx) && !defined(STM32F412Vx) && !defined(STM32F412Zx)
/* #define STM32F405xx */ /*!< STM32F405RG, STM32F405VG and STM32F405ZG */
/* #define STM32F415xx */ /*!< STM32F415RG, STM32F415VG and STM32F415ZG */
/* #define STM32F407xx */ /*!< STM32F407VG, STM32F407VF, STM32F407ZG,
                             STM32F407ZF, STM32F407IG and STM32F407IF */
/* #define STM32F417xx */ /*!< STM32F417VG, STM32F417VF, STM32F417ZG,
                             STM32F417ZF, STM32F417IG and STM32F417IF */
/* #define STM32F427xx */ /*!< STM32F427VI, STM32F427VG, STM32F427ZI,
                             STM32F427ZG, STM32F427II, STM32F427IG, STM32F427II
                             and STM32F427IG */
/* #define STM32F437xx */ /*!< STM32F437VI, STM32F437VG, STM32F437ZI,
                             STM32F437ZG, STM32F437II, STM32F437IG, STM32F437II
                             and STM32F437IG */
/* #define STM32F429xx */ /*!< STM32F429VI, STM32F429VG, STM32F429ZI,
                             STM32F429ZG, STM32F429II, STM32F429IG, STM32F429II,
                             STM32F429IG, STM32F429BK, STM32F429NK and
                             STM32F429II */
/* #define STM32F439xx */ /*!< STM32F439VI, STM32F439VG, STM32F439ZI,
                             STM32F439ZG, STM32F439II, STM32F439IG, STM32F439II,
                             STM32F439IG, STM32F439BK, STM32F439NK and
                             STM32F439II */
/* #define STM32F401xC */ /*!< STM32F401CB, STM32F401CC, STM32F401RB,
                             STM32F401RC, STM32F401VB and STM32F401VC */
/* #define STM32F401xE */ /*!< STM32F401CD, STM32F401RD, STM32F401VD,
                             STM32F401CE, STM32F401RE and STM32F401VE */
/* #define STM32F410Tx */ /*!< STM32F410T8 and STM32F410TB */
/* #define STM32F410Cx */ /*!< STM32F410C8 and STM32F410CB */
/* #define STM32F410Rx */ /*!< STM32F410R8 and STM32F410RB */
/* #define STM32F411xE */ /*!< STM32F411CD, STM32F411RD, STM32F411VD,
                             STM32F411CE, STM32F411RE and STM32F411VE */
#define STM32F446xx /*!< STM32F446MC, STM32F446ME, STM32F446RC, STM32F446RE,   \
                       STM32F446VC, STM32F446VE, STM32F446ZC and STM32F446ZE   \
                     */
/* #define STM32F469xx */ /*!< STM32F469AI, STM32F469II, STM32F469BI,
                             STM32F469NI, STM32F469AE, STM32F469IE, STM32F469BE
                             and STM32F469NE */
/* #define STM32F479xx */ /*!< STM32F479AI, STM32F479II, STM32F479BI,
                             STM32F479NI, STM32F479AE, STM32F479IE, STM32F479BE
                             and STM32F479NE */
/* #define STM32F412Cx */ /*!< STM32F412C8, STM32F412CB, STM32F412C8U and
                             STM32F412CBU */
/* #define STM32F412Rx */ /*!< STM32F412R8, STM32F412RB, STM32F412R8U and
                             STM32F412RBU */
/* #define STM32F412Vx */ /*!< STM32F412V8, STM32F412VB, STM32F412V8U and
                             STM32F412VBU */
/* #define STM32F412Zx */ /*!< STM32F412Z8, STM32F412ZB, STM32F412Z8U and
                             STM32F412ZBU */
#endif

/** @} */

extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */
extern const uint8_t AHBPrescTable[16]; /*!< AHB prescalers table values */
extern const uint8_t APBPrescTable[8];  /*!< APB prescalers table values */

/** @} */ /* End of group STM32F4xx_system */

/** @} */ /* End of group CMSIS */

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_STM32F4xx_H */
