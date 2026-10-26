class ElectraAnomaly_AreaAnomalyTrigger extends Trigger {
	private vector					m_ExtentMin;
	private vector					m_ExtentMax;
	private ref array< typename >	m_InsiderTypes = new array< typename >;
	
	private bool gl_bIsActive;

	private Object Electra_Idle;
	private Object Electra_Active;

	/* ~ [ Настройка аномалии ] ~ */
	private const float flAnomalyResetTime = 6.0;
	private const float flAnomalyDamageHealth = 99.0;
	private const float flAnomalyDamageShock = 50.0;
	private const float flAnomalyDamageNonPlayer = 999.0;

	/* ~ [ Защита артефактов ] ~ */
	private const float flArtifactFlashProtection = 0.40; // 60%
	private const float flArtifactCompassProtection = 0.70; // 30%
	private const float flArtifactMaxProtection = 27.72; // flAnomalyDamageHealth * flArtifactFlashProtection * flArtifactCompassProtection = flArtifactMaxProtection

	private ref Timer m_AnomalyResetTimer;

	void ElectraAnomaly_AreaAnomalyTrigger() {};

	void ~ElectraAnomaly_AreaAnomalyTrigger() {
		if ( Electra_Idle != null )
			GetGame().ObjectDelete( Electra_Idle );

		if ( Electra_Active != null )
			GetGame().ObjectDelete( Electra_Active );
	};

	void AddInsiderType( typename type ) {
		if ( type )
			m_InsiderTypes.Insert( type );
	};

	override void AddInsider( Object obj ) {
		if ( !obj.IsAnyInherited( m_InsiderTypes ) ) {
			return;
		}
		else super.AddInsider( obj );
	};

	override void SetExtents( vector mins, vector maxs ) {
		m_ExtentMax = maxs;
		m_ExtentMin = mins;

		super.SetExtents( mins, maxs );
	};

	override void OnEnter( Object obj ) {
		super.OnEnter( obj );

		if ( GetGame().IsServer() )
			OnEnterServer( obj );

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
			OnEnterClient( obj );
	};

	override void EOnTouch( IEntity other, int extra ) {
		Object obj;

		if ( Class.CastTo( obj, other ) ) {
			if ( obj )
				if ( obj.IsAlive() || obj.IsInherited( NR_Anomaly_Bolt ) || !obj.IsRuined() )
					AddInsider( obj );
		};
	};

	override void EOnInit( IEntity other, int extra ) {
		gl_bIsActive = false;
		
		Electra_Idle = GetGame().CreateObjectEx( "NR_Anomaly_ElectraIdle", this.GetPosition(), ECE_PLACE_ON_SURFACE );
	};

	void CTask__AnomalyReset() {
		gl_bIsActive = false;

		if ( Electra_Active != null ) GetGame().ObjectDelete( Electra_Active );
		Electra_Idle = GetGame().CreateObjectEx( "NR_Anomaly_ElectraIdle", this.GetPosition(), ECE_PLACE_ON_SURFACE );

		if ( m_insiders.Count() > 0 ) {
			if ( GetGame().IsServer() ) {
				for ( private int i = 0; i < m_insiders.Count(); i++ ) {
					TriggerInsider ins = m_insiders.Get( i );
					Object insObj = ins.GetObject();

					if ( ins.insider == null || !insObj.IsAlive() ) {
						m_insiders.Remove( i );
					};

					OnEnterServer( insObj );
				};
			};
		};
	};

	void OnEnterServer( Object obj )  {
		private	PlayerBase pPlayer = PlayerBase.Cast( obj );
		private	DayZCreature pCreature = DayZCreature.Cast( obj );
		private	NR_Anomaly_Bolt pBolt = NR_Anomaly_Bolt.Cast( obj );

		if ( pPlayer || pCreature || pBolt ) {
			if ( pPlayer.IsAlive() || pCreature.IsAlive() || !pBolt.IsRuined() ) {
				if ( !gl_bIsActive ) {
					gl_bIsActive = true;

					if ( Electra_Idle != null ) GetGame().ObjectDelete( Electra_Idle );
					Electra_Active = GetGame().CreateObjectEx( "NR_Anomaly_ElectraActive", this.GetPosition(), ECE_PLACE_ON_SURFACE );
				
					if ( !m_AnomalyResetTimer )
						m_AnomalyResetTimer = new Timer;

					if ( !m_AnomalyResetTimer.IsRunning() )
						m_AnomalyResetTimer.Run( flAnomalyResetTime, this, "CTask__AnomalyReset", NULL, false );
				
					if ( pPlayer ) {
						private float flDamageHealth = 0.0;
						private float flDamageShock = 0.0;

						private EntityAI pItemInHands = pPlayer.GetHumanInventory().GetEntityInHands();

						/* ~ [ Проверка в руках ] ~ */
						if ( pItemInHands && pItemInHands.IsKindOf( "NR_Artifact_Flash" ) ) {
							private ItemBase pFlash = ItemBase.Cast( pItemInHands );

							if ( !pFlash.IsRuined() ) {
								private float flQuantityFlash = pFlash.GetQuantity();

								if ( flQuantityFlash > 0.0 ) {
									flDamageHealth = flAnomalyDamageHealth * flArtifactFlashProtection;
									flDamageShock = flAnomalyDamageShock * flArtifactFlashProtection;
								};
							};
						};

						/* ~ [ Проверка в аттачментах ] ~ */
						private EntityAI pHips = pPlayer.GetInventory().FindAttachment( InventorySlots.HIPS );

						if ( pHips ) {
							if ( pHips.GetInventory().AttachmentCount() > 0 ) {
								for ( private int i = 0; i < pHips.GetInventory().GetAttachmentSlotsCount(); i++ ) {
									private EntityAI pAttachment = pHips.GetInventory().FindAttachment( pHips.GetInventory().GetAttachmentSlotId( i ) );

									if ( pAttachment && pAttachment.IsKindOf( "NR_Artifact_Flash" ) ) {
										private ItemBase pFlashSlot = ItemBase.Cast( pAttachment );

										if ( !pFlashSlot.IsRuined() ) {
											private float flQuantityFlashSlot = pFlashSlot.GetQuantity();

											if ( flQuantityFlashSlot > 0.0 ) {
												if ( flDamageHealth != 0.0 ) flDamageHealth *= flArtifactFlashProtection;
												else flDamageHealth = flAnomalyDamageHealth * flArtifactFlashProtection;

												if ( flDamageShock != 0.0 ) flDamageShock *= flArtifactFlashProtection;
												else flDamageShock = flAnomalyDamageShock * flArtifactFlashProtection;
											};
										};
									};
								};
							};
						};

						if ( flDamageHealth != 0.0 && flDamageHealth < flArtifactMaxProtection ) flDamageHealth = flArtifactMaxProtection;
						else flDamageHealth = flAnomalyDamageHealth;

						if ( flDamageShock != 0.0 && flDamageShock < flArtifactMaxProtection ) flDamageShock = flArtifactMaxProtection;
						else flDamageShock = flAnomalyDamageShock;

						pPlayer.AddHealth( "", "Health", -flDamageHealth );
						pPlayer.AddHealth( "", "Shock", -flDamageShock );
						pPlayer.SetSynchDirty();
						pPlayer.RequestSoundEvent( EPlayerSoundEventID.TAKING_DMG_HEAVY );
					}
					else if ( pCreature ) pCreature.AddHealth( "", "Health", -flAnomalyDamageNonPlayer );
					else if ( pBolt ) GetGame().ObjectDelete( pBolt );
				};
			};
		};
	};

	void OnEnterClient( Object obj ) {};

	override void OnLeave( Object obj ) {		
		if ( GetGame().IsServer() )
			OnLeaveServer( obj );

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
			OnLeaveClient( obj );

		super.OnLeave( obj );
	};

	void OnLeaveServer( Object obj ) {};
	void OnLeaveClient( Object obj ) {};
};