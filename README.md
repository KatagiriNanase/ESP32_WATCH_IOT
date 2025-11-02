# ESP32_WATCH_IOT
一个基于 ESP32 + LVGL + FreeRTOS 的智能手表项目，实现了多界面切换、天气显示、温湿度监测、MQTT物联网上报、~~二次元客制化主题~~。

> 本项目作为我简历上的一个项目的过程汇报   


## 项目简介

本项目旨在打造一个具有多功能交互界面的 ESP32 智能手表。
使用 **LVGL 9.3** 实现 UI，运行在 **FreeRTOS** 上，支持联网获取天气数据并通过 **OneNET IoT 平台** 上报传感器信息。

## 开发环境
>* **Platform:** ESP-IDF 5.3.4
>* **Language:** C (FreeRTOS + LVGL 9.3)
>* **Display:** ST7789 (240x280)
>* **Sensor:** DHT11
>* **Network:** WiFi + HTTP/MQTT
>* **IDE:** VSCode + ESP-IDF Plugin

## 功能进度总览

| 模块 | 状态 | 附加说明 |
|------|-----------|------|
|  **Clock 页面** | ✅ 已完成 |   |
|  **主界面（TileView）** | ✅ 已完成 |  |
|  **天气模块** | ✅ 已完成 |  |
|  **温湿度监测** | ✅ 已完成 |  |
|  **OneNET 数据上报** | ✅ 已完成 |  |
|  **UI 动画与滚动文本** | ✅ 已完成 |  |
|  **WiFi联网** | ✅ 已完成 |  |
|  **WS2812 GIF联动控制** | 🚧 开发中 |  |
|  **OTA 升级机制** | 🚧 开发中 |  |
|  **电量管理显示** | 🚧 开发中 |  |


## 📁 项目结构
```mermaid
graph TB
    %% 应用层 - 最上层
    subgraph AppLayer[📱 应用层]
        direction LR  %% 改为从左到右横向排布
        ClockApp[时钟应用]
        HomeApp[主界面应用]
        WeatherApp[天气应用]
        UIManager[UI管理器]
    end

    %% 服务层
    subgraph ServiceLayer[🔄 服务层]
        direction LR  %% 改为从左到右横向排布
        DataService[数据服务]
        NetworkService[网络服务]
        TimeService[时间服务]
        UIService[UI服务]
    end

    %% 中间件层
    subgraph MiddlewareLayer[⚙️ 中间件层]
        direction LR  %% 改为从左到右横向排布
        LVGL[图形框架 LVGL]
        MQTT[MQTT客户端]
        HTTP[HTTP客户端]
        FS[文件系统]
    end

    %% 驱动层
    subgraph DriverLayer[🔧 驱动层]
        direction LR  %% 改为从左到右横向排布
        DisplayDrv[显示驱动]
        TouchDrv[触摸驱动]
        SensorDrv[传感器驱动]
        NetworkDrv[网络驱动]
    end

    %% 硬件层 - 最下层
    subgraph HardwareLayer[💾 硬件层]
        direction LR  %% 改为从左到右横向排布
        MCU[ESP32 MCU]
        Display[显示屏]
        Touch[触摸屏]
        Sensor[传感器]
        WiFi[WiFi模块]
    end

    %% 外部服务
    subgraph CloudLayer[☁️ 云服务]
        direction LR  %% 改为从左到右横向排布
        IoTPlatform[物联网平台]
        WeatherAPI[天气API]
        NTPServer[NTP服务]
    end

    %% 数据流向 - 从上到下
    AppLayer --> ServiceLayer
    ServiceLayer --> MiddlewareLayer
    MiddlewareLayer --> DriverLayer
    DriverLayer --> HardwareLayer
    
    %% 外部连接
    NetworkService --> CloudLayer
    MQTT --> IoTPlatform
    HTTP --> WeatherAPI
    TimeService --> NTPServer

    %% 样式
    classDef app fill:#e3f2fd
    classDef service fill:#f3e5f5
    classDef middleware fill:#e8f5e8
    classDef driver fill:#fff3e0
    classDef hardware fill:#ffebee
    classDef cloud fill:#fce4ec

    class AppLayer app
    class ServiceLayer service
    class MiddlewareLayer middleware
    class DriverLayer driver
    class HardwareLayer hardware
    class CloudLayer cloud
```

## 📦 未来计划

* [ ] 添加 BLE 控制功能
* [ ] 增加表盘切换动画
* [ ] 图片资源管理
* [ ] 小说阅读器
* [ ] 视频播放器
* [ ] 自定义部件、装饰、主题

---

