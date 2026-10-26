class NR_Anomaly_ElectraIdle extends House {
	private AnomalyElectraLightIdle0		m_Light;
	private Particle 					m_AnomalyParticle;
	private SoundOnVehicle				m_AnomalySound;
	
	void NR_Anomaly_ElectraIdle() {
		#ifndef SERVER
		m_Light = AnomalyElectraLightIdle0.Cast( ScriptedLightBase.CreateLight( AnomalyElectraLightIdle0, "0 1 0", 0.5 ) );
		m_Light.AttachOnObject( this, "0 1 0", "0 0 0" );

		m_AnomalySound = PlaySoundLoop( "Electra_Idle", 30 );

		m_AnomalyParticle = Particle.PlayOnObject( ParticleList.ANOMALY_ELECTRA_IDLE, this );
		#endif
	};
	
	void ~NR_Anomaly_ElectraIdle() {
		if ( m_Light ) m_Light.FadeOut();
		if ( m_AnomalyParticle ) m_AnomalyParticle.Stop();
		if ( m_AnomalySound ) GetGame().ObjectDelete( m_AnomalySound );
 	};
};

class AnomalyElectraLightIdle0 extends PointLightBase {		
	void AnomalyElectraLightIdle0() {
		SetVisibleDuringDaylight( true );
		SetRadiusTo( 6 );
		SetBrightnessTo( 1.5 );
		SetCastShadow( false );
		SetFadeOutTime( 3 );
		SetDiffuseColor( 0.2, 0.5, 1.0 );
		SetAmbientColor( 0.2, 0.5, 1.0 );
		SetFlareVisible( false );
		SetFlickerAmplitude( 0.7 );
		SetFlickerSpeed( 4.0 );
		SetDancingShadowsMovementSpeed( 0.4 );
		SetDancingShadowsAmplitude( 0.5 );
	};
};