class NR_DetectorWorldManager
{
    private static ref NR_DetectorWorldManager m_Instance;
    private ref array<Object> m_ActiveAnomalies;
    
    // Приватный конструктор (синглтон)
    private void NR_DetectorWorldManager()
    {
        m_ActiveAnomalies = new array<Object>();
    }
    
    static NR_DetectorWorldManager GetInstance()
    {
        if (!m_Instance)
        {
            m_Instance = new NR_DetectorWorldManager();
        }
        return m_Instance;
    }
    
    void RegisterAnomaly(Object anomaly)
    {
        if (!anomaly) return;
        if (m_ActiveAnomalies.Find(anomaly) == -1)
        {
            m_ActiveAnomalies.Insert(anomaly);
        }
    }
    
    void UnregisterAnomaly(Object anomaly)
    {
        if (!anomaly) return;
        int index = m_ActiveAnomalies.Find(anomaly);
        if (index != -1)
        {
            m_ActiveAnomalies.Remove(index);
        }
    }
    
    array<Object> GetActiveAnomalies()
    {
        return m_ActiveAnomalies;
    }
}