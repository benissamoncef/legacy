/*E*/
/*  Fichier : $Id: eopc_dir.h,v 1.1 2021/05/04 13:53:13 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/05/04 13:53:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER eopc_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TEOPC
*
------------------------------------------------------
* HISTORIQUE :
*
* Minet.S	version 1.0	12 Mar 2020 : Creation
*
------------------------------------------------------ */

#ifndef eopc_dir
#define eopc_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include 	"eopc_opc.h"
#include	"eopc_cfg.h"
#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

/* definitions de constantes exportees */

/* definitions de types exportes */
typedef struct list_EopcContext
{
			XDY_Eqt		NumBICE;
			XDY_Texte	EndPointOPCUA;
			XDY_Texte	UserOPCUA;
			XDY_Texte	PasswordOPCUA;
			XDY_Entier	SousType;
			XDY_NomMachine	NomMachine;
			EopcContext	Context;																
} list_EopcContext;



//TODO a virer ?
typedef struct {
	XDY_AdresseIP	adresse;
	XDY_PortIP	port;
	char		tampon[1000];
} T_Tampon;
#define C_MAX_TAMPON	50
 XZEXT_ST_Sockets	vg_DesSocks;			/* struct. descs. sockets typiques tache TELxi */
 XZEXT_Liste_Cmdes_LCR_IP	*pg_ListeCmdesLCR;	/* liste cmdes. LCR empilees */
 XZEXT_IP_Config	vg_Config;			/* config tache TELxi */
XZEXT_IP_Config		vg_ConfigE;			/* struct config tache */	
 XZEXT_MASK	*vg_SockList ;			/* liste globale des sockets */
 int		vg_EtatST;
XDY_Entier vg_nb_connexion= 0;
list_EopcContext vg_list_EopcContext[100];

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
