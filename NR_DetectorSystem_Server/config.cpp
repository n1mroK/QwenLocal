class CfgPatches {
	class NR_DetectorSystem_Server {
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = { "DZ_Data", "NR_DetectorSystem_Client" };
	};
};

class CfgMods {
	class NR_DetectorSystem_Server {
		name = "Detector System | Server Side"; 								// name
		picture = "";			// picture in expanded description
		logoSmall = "";	// icon next to mod name when description is not expanded
		logo = "";				// logo below game menu
		logoOver = "";		// on mouse hover over logo
		tooltip = "";							// tool tip on mouse hover
		overview = "Discord: n1mrok"; 						// overview
		action = "https://steamcommunity.com/id/n1mroK/";					// link
		author = "nimroK";								// author
		version = "1.0";								// version
		type = "mod";
		dependencies[] = { "World", "Mission" };
		
		class defs {
			class worldScriptModule {
				value = "";
				files[] = { "NR_DetectorSystem_Server/Scripts/4_World" };
			};
	
			class missionScriptModule {
				value = "";
				files[] = { "NR_DetectorSystem_Server/Scripts/5_Mission" };
			};
		};
	};
};