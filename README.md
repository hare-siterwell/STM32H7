# STM32H7_Demo
STM32H7 project demo.

# Getting Started
## .vscode/launch.json (optional)
```
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "pyOCD",
      "type": "cortex-debug",
      "request": "launch",
      "cwd": "${workspaceFolder}",
      "servertype": "pyocd",
      "executable": "build/release.elf",
      "runToMain": true,
      "svdFile": "STM32H743.svd", // svd
      "targetId": "STM32H743VITx", // target
      "cmsisPack": "Keil.STM32H7xx_DFP.2.8.0.pack" // pack
    }
  ]
}
```

## .vscode/tasks.json (optional)
```
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Load",
      "type": "shell",
      "command": "pyocd",
      "args": [
        "flash",
        "--erase",
        "sector",
        "--target",
        "STM32H743VITx", // target
        "--pack",
        "Keil.STM32H7xx_DFP.2.8.0.pack", // pack
        "MDK-ARM/Objects/release.hex"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

# Tree
```
.
├── CMakeLists.txt # CMake调用工具链
├── Components
│   ├── BSP # 主要代码
│   │   ├── bsp_app.c
│   │   ├── bsp_app.h
│   │   ├── bsp_delay.c
│   │   ├── bsp_delay.h
│   │   ├── bsp_lcd.c
│   │   ├── bsp_lcd.h
│   │   ├── bsp_led.c
│   │   ├── bsp_led.h
│   │   ├── bsp_spi.c
│   │   ├── bsp_spi.h
│   │   ├── bsp_usart.c
│   │   └── bsp_usart.h
│   ├── Devices # 硬件驱动代码
│   │   ├── STEP
│   │   │   ├── step.c
│   │   │   └── step.h
│   │   └── TFTLCD
│   │       ├── font.h
│   │       ├── image.h
│   │       ├── tftlcd.c
│   │       └── tftlcd.h
│   ├── uCOS-III
│   └── CMSIS-RTOS2 # RTX5实时操作系统
├── Core
│   ├── Inc
│   └── Src
│       ├── main.c # 适配RTOS
│       └── stm32h7xx_it.c # 中断相关代码
├── Drivers # 不允许更改
│   ├── CMSIS
│   └── STM32H7xx_HAL_Driver
├── MDK-ARM # 简单烧录调试可用pyOCD实现
│   ├── DebugConfig
│   ├── Objects
│   ├── STM32V7.uvoptx
│   ├── STM32V7.uvprojx
│   └── startup_stm32h743xx.s
├── README.md
├── STM32V7.ioc
└── keilkill.bat # 删除多余文件，方便打包
```
