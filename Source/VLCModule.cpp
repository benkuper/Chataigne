/*
==============================================================================

MilluminModule.cpp
Created: 17 May 2017 7:41:53pm
Author:  Ben

==============================================================================
*/

#include "VLCModule.h"
#include "SendTCPStringCommand.h"

VLCModule::VLCModule() :
	TCPModule(getDefaultTypeString())
{

	NLOG("VLC", "Notice:\nTo be able to control VLC via Remote Control, you must launch the app with rc options:\nExample for launching in windows on port 50000 :\nvlc.exe --rc-host localhost:50000");

	defManager.add(CommandDefinition::createDef(this, "Media", "Play", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue","play"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Stop", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue","stop"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Pause", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "pause"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Rewind", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "rewind"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Faster Speed", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "faster"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Slower Speed", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "slower"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Normal Speed", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "normal"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Frame by frame", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "frame"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Next", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "next"));
	defManager.add(CommandDefinition::createDef(this, "Media", "Prev", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "prev"));

	defManager.add(CommandDefinition::createDef(this, "Playlist", "Clear", &SendTCPStringCommand::create, CommandContext::ACTION)->addParam("fixedValue", "clear"));


	/*
	S: help
+----[ Commandes de l'interface à distance ]
| 
| add XYZ  . . . .  ajoute XYZ à la playlist et le lit
| enqueue XYZ  . . . . . . .  ajoute XYZ à la playlist
| liste de lecture . . .  afficher les éléments de la liste
| play . . . . . . . . . . . . . . . . . . . . . jouer
| stop . . . . . . . . . . . . . . . . . . . . arrêter le flux
| next . . . . . . . . . . . . . . . . élément suivant
| prev . . . . . . . . . . . . . . . élément précédent
| goto . . . . . . . . . . . . . . aller à la position
| repeat [on|off]. . . .  commuter la répétition d'un élément
| loop [on|off]. . . . .  commuter le rebouclage de liste
| random [on|off]. . .  commuter le saut aléatoire
| clear . . . . . . . . .  effacer la liste de lecture
| status . . . . . . . . . . afficher les informations
| title [X] . . . . afficher/modifier le titre courant
| title_n  . . . . . . . . . . . . . . . titre suivant
| title_p  . . . . . . . . . . . . . . titre précédent
| chapter [X]  . . . . . modifier/afficher le chapitre
| chapter_n . . . . . . . . . . . . . chapitre suivant
| chapter_p . . . . . . . . . . . . chapitre précédent
| 
| seek X . . . se déplacer, en secondes, ex. « seek 12 »
| pause . . . . . . . . . . . . . . . . commuter la pause
| fastforward . . . . . . . . . fixé à vitesse maximum
| rewind . . . . . . . . . . . . . . . . .  rembobiner
| faster . . . . . . . . . . . . . . . jouer plus vite
| slower . . . . . . . . . . . .  jouer plus lentement
| normal . . . . . . . . . . . jouer à vitesse normale
| frame  . . . . . . . . . . . . jouer image par image
| f [on|off] . . . . . . . . . . . . commuter le plein écran
| info . . . . . . .  informations sur le flux courant
| stats . . . . . . . . . .  afficher les informations
| get_time . .  temps parcouru depuis le début du flux
| is_playing . . . . 1 si un élément est joué, 0 sinon
| get_title . . . .  afficher le titre du flux courant
| get_length . . . . . . . la longueur du flux courant
| 
| volume [X] . . . . . . . modifier/afficher le volume
| volup [X]  . . . . . . . .  augmenter le volume de X
| voldown [X]  . . . . . . . . diminuer le volume de X
| adev [périph] . . modifier/afficher le périphérique audio
| achan [X] . . . . modifier/afficher les canaux audio
| atrack [X] . . . . .modifier/afficher la piste audio
| vtrack [X] . . . .  modifier/afficher la piste vidéo
| vratio [X] . . modifier/afficher la proportion vidéo
| vcrop [X] . . . . modifier/afficher le rognage vidéo
| vzoom [X] . . . . .  modifier/afficher le zoom vidéo
| snapshot . . . . . . . . prendre une capture d'écran
| strack [X] .  modifier/afficher la piste sous-titres
| key [raccourci] . . .  simuler l'appui sur raccourci
| 
| help . . . . . . . . . . . . . . . ce message d'aide
| logout . . . . .  quitte l'interface sans fermer VLC
| quit . . . . . . . . . . . . . . . . . .  quitte VLC
| 
+----[ fin de l'aide ]

	*/

}