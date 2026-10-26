class NR_ArtifactSpawnChance
{
    string ClassName;
    float Chance;
}

class NR_AnomalyConfig
{
    string AnomalyClassName;
    ref array<ref NR_ArtifactSpawnChance> Artifacts;
}

class NR_DetectorConfigEntry
{
    string ClassName;
    float DetectionDistance;
    float SpawnDistance;
    ref array<ref NR_AnomalyConfig> Anomalies;
}

class NR_DetectorConfig
{
    ref array<ref NR_DetectorConfigEntry> Detectors;
    
    // --------------------------------------------------------------------------------------------
    // Загрузка конфига из JSON
    // --------------------------------------------------------------------------------------------
    
    static NR_DetectorConfig Load()
    {
        string fileName = "$profile:NR_DetectorSystem/NR_DetectorSystem.json";
        NR_DetectorConfig config = new NR_DetectorConfig();
        
        if (!FileExist(fileName))
        {
            Print("[NR Detector] Config not found, creating default");
            config = CreateDefaultConfig();
            config.Save();
            return config;
        }
        
        FileHandle file = OpenFile(fileName, FileMode.READ);
        if (!file)
        {
            Print("[NR Detector] ERROR: Failed to open config file");
            return CreateDefaultConfig();
        }
        
        // Читаем весь файл сразу
        string fileContent;
        string line;
        
        while (FGets(file, line) > 0)
        {
            fileContent += line;
        }
        
        CloseFile(file);
        
        if (fileContent == "")
        {
            Print("[NR Detector] ERROR: Config file is empty");
            return CreateDefaultConfig();
        }
        
        // Парсим JSON
        JsonSerializer serializer = new JsonSerializer();
        string errorMessage;
        
        if (serializer.ReadFromString(config, fileContent, errorMessage))
        {
            Print("[NR Detector] Config loaded from " + fileName);
            return config;
        }
        
        Print("[NR Detector] ERROR parsing JSON: " + errorMessage);
        return CreateDefaultConfig();
    }
    
    // --------------------------------------------------------------------------------------------
    // Сохранение конфига
    // --------------------------------------------------------------------------------------------
    
    void Save()
    {
        string fileName = "$profile:NR_DetectorSystem/NR_DetectorSystem.json";
        string directory = "$profile:NR_DetectorSystem";
        
        // Создаем папку если её нет
        if (!FileExist(directory))
        {
            MakeDirectory(directory);
        }
        
        // Сериализуем в JSON
        JsonSerializer serializer = new JsonSerializer();
        string fileContent;
        serializer.WriteToString(this, true, fileContent);
        
        // Сохраняем файл
        FileHandle file = OpenFile(fileName, FileMode.WRITE);
        if (!file)
        {
            Print("[NR Detector] ERROR: Failed to save config file");
            return;
        }
        
        FPrint(file, fileContent);
        CloseFile(file);
        
        Print("[NR Detector] Config saved to " + fileName);
    }
    
    // --------------------------------------------------------------------------------------------
    // Создание дефолтного конфига
    // --------------------------------------------------------------------------------------------
    
    static NR_DetectorConfig CreateDefaultConfig()
    {
        NR_DetectorConfig config = new NR_DetectorConfig();
        config.Detectors = new array<ref NR_DetectorConfigEntry>();
        
        // Конфиг для Echo детектора
        NR_DetectorConfigEntry echoEntry = new NR_DetectorConfigEntry();
        echoEntry.ClassName = "NR_Detector_Echo";
        echoEntry.DetectionDistance = 150.0;  // Увеличено для плавности
        echoEntry.SpawnDistance = 3.0;
        echoEntry.Anomalies = new array<ref NR_AnomalyConfig>();
        
        // Электра аномалия
        NR_AnomalyConfig electraConfig = new NR_AnomalyConfig();
        electraConfig.AnomalyClassName = "ElectraAnomaly1_AreaAnomalyTrigger";
        electraConfig.Artifacts = new array<ref NR_ArtifactSpawnChance>();
        
        // Артефакты для Электры
        NR_ArtifactSpawnChance medusa = new NR_ArtifactSpawnChance();
        medusa.ClassName = "NR_Artifact_Medusa";
        medusa.Chance = 0.5;
        electraConfig.Artifacts.Insert(medusa);
        
        NR_ArtifactSpawnChance stoneFlower = new NR_ArtifactSpawnChance();
        stoneFlower.ClassName = "NR_Artifact_StoneFlower";
        stoneFlower.Chance = 0.3;
        electraConfig.Artifacts.Insert(stoneFlower);
        
        NR_ArtifactSpawnChance nightStar = new NR_ArtifactSpawnChance();
        nightStar.ClassName = "NR_Artifact_NightStar";
        nightStar.Chance = 0.15;
        electraConfig.Artifacts.Insert(nightStar);
        
        NR_ArtifactSpawnChance vyvert = new NR_ArtifactSpawnChance();
        vyvert.ClassName = "NR_Artifact_Vyvert";
        vyvert.Chance = 0.05;
        electraConfig.Artifacts.Insert(vyvert);
        
        echoEntry.Anomalies.Insert(electraConfig);
        config.Detectors.Insert(echoEntry);
        
        return config;
    }
}