modded class NR_Anomaly_ElectraBase {
	private ref Timer m_AnomalyAppearTimer;
	private const float flAnomalyAppearTime = 0.1;

	private ElectraAnomaly_AreaAnomalyTrigger m_Trigger;
	private Object m_Anomaly_Trigger;

	void NR_Anomaly_ElectraBase() {
		if ( !m_AnomalyAppearTimer )
			m_AnomalyAppearTimer = new Timer;

		if ( !m_AnomalyAppearTimer.IsRunning() )
			m_AnomalyAppearTimer.Run( flAnomalyAppearTime, this, "CTask__AnomalyAppear", NULL, false );
	};

	/*void ~NR_Anomaly_ElectraBase() {
		if ( m_Anomaly_Trigger != null ) {
			NR_DetectorWorldManager.GetInstance().UnregisterAnomaly( m_Anomaly_Trigger );
			GetGame().ObjectDelete( m_Anomaly_Trigger );
		};
	};*/

	override void EEDelete( EntityAI parent ) {
		super.EEDelete( parent );

		if ( m_AnomalyAppearTimer ) {
			m_AnomalyAppearTimer.Stop();
			m_AnomalyAppearTimer = null;
		};

		if ( m_Anomaly_Trigger ) {
			NR_DetectorWorldManager.GetInstance().UnregisterAnomaly( m_Anomaly_Trigger );

			GetGame().ObjectDelete( m_Anomaly_Trigger );

			m_Anomaly_Trigger = null;
		};
	};

	void CTask__AnomalyAppear() {
		if ( Math.RandomIntInclusive( 0, 1 ) == 1 ) {
			m_Anomaly_Trigger = GetGame().CreateObjectEx( "ElectraAnomaly_AreaAnomalyTrigger", this.GetPosition(), ECE_NOSURFACEALIGN );

			if ( m_Anomaly_Trigger ) {
				if ( Class.CastTo( m_Trigger, m_Anomaly_Trigger ) ) {
					m_Trigger.SetPosition( this.GetPosition() );
					m_Trigger.SetOrientation( this.GetOrientation() );
					m_Trigger.SetExtents( "-2 -0.5 -2", "2 0.5 2" );							
					m_Trigger.AddInsiderType( Man );
					m_Trigger.AddInsiderType( DayZCreature );
					m_Trigger.AddInsiderType( NR_Anomaly_Bolt );

					if ( Math.RandomIntInclusive( 0, 1 ) == 1 )
						NR_DetectorWorldManager.GetInstance().RegisterAnomaly( m_Anomaly_Trigger );
				};
			};
		};
	};
};