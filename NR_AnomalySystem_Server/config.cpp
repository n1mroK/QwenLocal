class CfgPatches {
	class NR_AnomalySystem_Server {
		units[] = {};
		weapons[] = {};
		requiredVersion = 1;
		requiredAddons[] = { "DZ_Data", "NR_AnomalySystem_Client" };
	};
};

class CfgMods {
	class NR_AnomalySystem_Server {
		name = "Anomaly System | Server Side"; 								// name
		picture = "";			// picture in expanded description
		logoSmall = "";	// icon next to mod name when description is not expanded
		logo = "";				// logo below game menu
		logoOver = "";		// on mouse hover over logo
		tooltip = "";							// tool tip on mouse hover
		overview = "Discord: n1mrok"; 						// overview
		action = "https://steamcommunity.com/id/n1mrok/";					// link
		author = "nimroK";								// author
		version = "1.0";								// version
		type = "mod";
		dependencies[] = { "World" };
		
		class defs {
			class worldScriptModule {
				value = "";
				files[] = { "NR_AnomalySystem_Server/Scripts/4_World" };
			};
		};
	};
};