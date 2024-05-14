/*E*/
/*Fichier : $Id: xzao730.h,v 1.1 2016/10/13 14:56:14 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2016/10/13 14:56:14 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao730.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05: Creation
------------------------------------------------------ */

#ifndef xzao730
#define xzao730

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO730_RPC_NAME "XZAO730"

#define XZAOC_XZAO730_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
	XDY_Eqt	Numero;
   	XDY_NomEqt	NomEqtSAGA;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
	XDY_Octet	TypeSAGA;
	char		Instance[30];
	char		FelsCode[5];
	char		EqtCode[5];
	char		CodeCmdMessage[5];
	char		CodeCmdLuminosite[5];
	char		CodeCmdPicto[5];
	char		CodeCmdHeure[5];
	XDY_NomSite	NomSite;
	XDY_Octet	NumeroSite;
	char		Alarme[50];
	char		CodeCmdBandeau[5];
} XZAOT_ConfEqtSAGA;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO730_Ajouter_Eqt_SAGA (	XDY_Basedd,
					XZAOT_ConfEqtSAGA,
					XDY_ResConf *);

#endif
