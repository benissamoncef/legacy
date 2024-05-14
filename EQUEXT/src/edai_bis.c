/*E*/
/*Fichier :  $Id: edai_bis.c,v 1.2 2021/05/04 13:25:02 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2021/05/04 13:25:02 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_bis.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Fonction utilitaire de gestion de la redondance des DAI
*
------------------------------------------------------
* HISTORIQUE :
*
* BOULAY N.	29/09/20 : Ajout deuxieme adresse IP DEM-SAE175 1.20
*/

#include "edai_don.h"
#include "ex_mlcr.h"
#include "edai_bis.h"
/* 
 * SERVICE RENDU :
 * Un DAI ne répond plus, on utilise l'autre
 * 
 * ARGUMENTS EN ENTREE :    
 * moyen identification du DAI en faute
 * 
*/

void switchDAI(char curr_adrrgs[]) {   
  EDAI_DONNEES_DAI		*pl_listeDAI;
XZEXT_Trame          pl_mlcr;

  for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant ) {
    
    if (!strcmp(pl_listeDAI->Config.AdresseRGS,curr_adrrgs))  { 
	pl_listeDAI->Config.DAIBis = ! pl_listeDAI->Config.DAIBis;
sprintf(pl_mlcr," ST ID=%s\0", vg_idpassw);
	EnvoyerTrameIP2(
                                   pl_mlcr,
                                   XZEXC_TRAME_STAT_COMP,
                                   pl_listeDAI->Config.AdresseIP,
                                   pl_listeDAI->Config.Port,
                                   pl_listeDAI->Config.Adresse2IP,
                                   pl_listeDAI->Config.Port2,
                                   pl_listeDAI->Config.DAIBis,
                                   pl_listeDAI->Config.AdresseRGS,
                                  XZEXC_NOCMD,
                                   XZEXC_PRIORITEFAIBLE,
                                   pl_listeDAI->SocketMaitre);
	EnvoyerTrameIP2(
                                   pl_mlcr,
                                   XZEXC_TRAME_STAT_COMP,
                                   pl_listeDAI->Config.AdresseIPE,
                                   pl_listeDAI->Config.PortE,
                                   pl_listeDAI->Config.Adresse2IPE,
                                   pl_listeDAI->Config.Port2E,
                                   pl_listeDAI->Config.DAIBis,
                                   pl_listeDAI->Config.AdresseRGS,
                                  XZEXC_NOCMD,
                                   XZEXC_PRIORITEFAIBLE,
                                   pl_listeDAI->SocketMaitre);


	break;
    }
  }
}
/*
 * Remplace la fonction EnvoyerTrameIP pour EDAI,
 * adr2 et port2 prendront les valeurs Port2, Adresse2IP, Port2E, Adresse2IPE.
 * DAIBis use Adresse2 
 * Est-ce que Adresse2IPE et Adresse2IP sont lié ? dans le cas contraire, DAIBis doit être dupliqué.
*/ 
int  EnvoyerTrameIP2 ( XZEXT_Trame	trame,
		       int	type_msg,
		       char*	adr,
		       int      port,
		       char*	adr2,
		       int      port2,
		       int      DAIBis,
		       char*	rgs,
		       int 	nocmd,
		       int 	priorite,
		       int	socket) {
  if(!DAIBis) {
    EnvoyerTrameIP(trame, type_msg, adr, port, rgs, nocmd,priorite,socket);
  } else {
    EnvoyerTrameIP(trame, type_msg, adr2, port2, rgs, nocmd,priorite,socket);
  }
}
