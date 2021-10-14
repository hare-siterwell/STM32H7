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
