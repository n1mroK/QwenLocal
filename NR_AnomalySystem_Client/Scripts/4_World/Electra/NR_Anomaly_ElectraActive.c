class NR_Anomaly_ElectraActive extends House {
	private AnomalyElectraLightActive0	m_Light;
	private Particle 					m_AnomalyParticle;
	private SoundOnVehicle				m_AnomalySound;
	
	void NR_Anomaly_ElectraActive() {
		#ifndef SERVER
		m_Light = AnomalyElectraLightActive0.Cast( ScriptedLightBase.CreateLight( AnomalyElectraLightActive0, "0 1 0" ) );
		m_Light.AttachOnObject( this, "0 1 0", "0 0 0" );

		m_AnomalySound = PlaySound( "Electra_Active", 30 );

		m_AnomalyParticle = Particle.PlayOnObject( ParticleList.ANOMALY_ELECTRA_ACTIVE, this );
		#endif
	};
	
	void ~NR_Anomaly_ElectraActive() {
		if ( m_Light ) m_Light.FadeOut();
		if ( m_AnomalyParticle ) m_AnomalyParticle.Stop();
		if ( m_AnomalySound ) GetGame().ObjectDelete( m_AnomalySound );
 	};
};

class AnomalyElectraLightActive0 extends PointLightBase {
	void AnomalyElectraLightActive0() {
		SetVisibleDuringDaylight( true );
		SetRadiusTo( 150 );
		SetBrightnessTo( 500 );
		SetFlareVisible( false );
		SetAmbientColor( 0.2, 0.2, 1.0 );
		SetDiffuseColor( 0.2, 0.2, 1.0 );
		SetFadeOutTime( 0.15 );
		SetLifetime( 0.4 );
		SetDisableShadowsWithinRadius( -1 );
	};
};