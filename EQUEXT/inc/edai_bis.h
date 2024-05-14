/*E*/
/*Fichier :  $Id: edai_bis.h,v 1.1 2020/11/04 08:45:18 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2020/11/04 08:45:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_bis.h
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
#ifndef EDAI_BIS
#define EDAI_BIS

void switchDAI(char curr_adrrgs[]);
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
		       int	socket);
#endif
