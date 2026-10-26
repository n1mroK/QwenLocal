modded class MissionServer
{
    private ref NR_DetectorConfig m_DetectorConfig;
    
    override void OnInit()
    {
        super.OnInit();
        m_DetectorConfig = NR_DetectorConfig.Load();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateDetectors, 150, true);
    }
    
    // Получаем список аномалий из World-менеджера
    private array<Object> GetActiveAnomalies()
    {
        return NR_DetectorWorldManager.GetInstance().GetActiveAnomalies();
    }
    
    // Поиск ближайшей аномалии
    private Object FindNearestAnomaly(vector playerPos, string detectorType, out float minDist)
    {
        minDist = float.MAX;
        Object nearest = null;
        NR_DetectorConfigEntry config = GetDetectorConfig(detectorType);
        if (!config) return null;
        
        float maxDist = config.DetectionDistance;
        array<Object> anomalies = GetActiveAnomalies();
        
        for (int i = 0; i < anomalies.Count(); i++)
        {
            Object anomaly = anomalies.Get(i);
            if (!anomaly) continue;
            
            float dist = vector.Distance(playerPos, anomaly.GetPosition());
            if (dist < minDist && dist <= maxDist)
            {
                minDist = dist;
                nearest = anomaly;
            }
        }
        return nearest;
    }
    
    private NR_DetectorConfigEntry GetDetectorConfig(string className)
    {
        if (!m_DetectorConfig || !m_DetectorConfig.Detectors) return null;
        foreach (NR_DetectorConfigEntry entry : m_DetectorConfig.Detectors)
        {
            if (entry && entry.ClassName == className)
                return entry;
        }
        return null;
    }
    
    // Основной цикл обновления детекторов
    private void UpdateDetectors()
    {
        if (!GetGame().IsServer()) return;
        
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        
        // Если есть игроки
        if (players.Count() > 0)
        {
            foreach (Man man : players)
            {
                PlayerBase player = PlayerBase.Cast(man);
                if (!player) continue;
                
                NR_Detector_Echo detector = GetDetectorInHands(player);
                if (detector && detector.GetCompEM().IsWorking())
                    ProcessDetector(player, detector);
            }
        }
    }
    
    private NR_Detector_Echo GetDetectorInHands(PlayerBase player)
    {
        EntityAI item = player.GetHumanInventory().GetEntityInHands();
        if (item)
            return NR_Detector_Echo.Cast(item);
        return null;
    }
    
    private void ProcessDetector(PlayerBase player, NR_Detector_Echo detector)
    {
        float minDist;
        Object nearest = FindNearestAnomaly(player.GetPosition(), detector.GetType(), minDist);
        
        // Если нет аномалии – отправляем нулевой сигнал и выходим
        if (!nearest)
        {
            SendSignal(detector, 0.0);
            return;
        }
        
        NR_DetectorConfigEntry config = GetDetectorConfig(detector.GetType());
        if (!config) return;
        
        float t = minDist / config.DetectionDistance;
        if (t > 1.0) t = 1.0;
        
        float strength = 1.0 - (t * t); // квадратичная формула
        if (strength < 0.0) strength = 0.0;
        if (strength > 1.0) strength = 1.0;
        
        bool shouldSpawn = false;
        string artifactToSpawn = "";
        if (minDist <= config.SpawnDistance)
        {
            foreach (NR_AnomalyConfig anomConfig : config.Anomalies)
            {
                if (anomConfig && anomConfig.AnomalyClassName == nearest.GetType())
                {
                    artifactToSpawn = ChooseArtifactByChance(anomConfig.Artifacts);
                    shouldSpawn = true;
                    break;
                }
            }
        }
        
        SendSignal(detector, strength);
        
        Print("[NR_DetectorySystem_Server] Дошёл до спавна");

        if (shouldSpawn && artifactToSpawn != "")
        {
            Print("[NR_DetectorySystem_Server] Прошёл проверку на спавн");
            SpawnArtifact(player, artifactToSpawn, nearest);
        }
    }
    
    private void SendSignal(NR_Detector_Echo detector, float strength)
    {
        if (!detector) return;
        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(strength);
        rpc.Send(detector, NR_eDetectorRPCs.RPC_DETECTOR_SIGNAL, true, NULL);
    }
    
    private void SpawnArtifact(PlayerBase player, string artifactClass, Object anomaly)
    {
        Print("[NR_DetectorySystem_Server] Спавн арта - не прошел проверку на игрока или арт");
        if (!player || artifactClass == "") return;
        Print("[NR_DetectorySystem_Server] Спавн арта - прошел проверку на игрока или арт");
        
        vector pos = player.GetPosition();
        pos[1] = pos[1] + 0.5;
        
        Object artifact = GetGame().CreateObject(artifactClass, pos);
        if (artifact)
        {
            Print("[NR_DetectorySystem_Server] Спавн арта - нашел арт");

            vector newPos = artifact.GetPosition();
            newPos[0] = newPos[0] + Math.RandomFloat(-0.5, 0.5);
            newPos[2] = newPos[2] + Math.RandomFloat(-0.5, 0.5);
            artifact.SetPosition(newPos);

            // Отправляем звук появления
            ScriptRPC rpc = new ScriptRPC();
            rpc.Write(1);
            rpc.Send(artifact, NR_eDetectorRPCs.RPC_ARTIFACT_SPAWN, true, NULL);
            
            // Убираем аномалию через World-менеджер
            if (anomaly)
            {
                Print("[NR_DetectorySystem_Server] Спавн арта - удалил аномалию после спавна арта");
                NR_DetectorWorldManager.GetInstance().UnregisterAnomaly(anomaly);
            }
            else Print("[NR_DetectorySystem_Server] Спавн арта - не увидел аномалию");
        }
        else Print("[NR_DetectorySystem_Server] Спавн арта - не увидео арт");
    }
    
    private string ChooseArtifactByChance(array<ref NR_ArtifactSpawnChance> artifacts)
    {
        if (!artifacts || artifacts.Count() == 0) return "";
        float r = Math.RandomFloat(0, 1);
        float cum = 0.0;
        foreach (NR_ArtifactSpawnChance a : artifacts)
        {
            cum += a.Chance;
            if (r <= cum)
                return a.ClassName;
        }
        return artifacts.Get(0).ClassName;
    }
}