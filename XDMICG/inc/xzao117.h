/*E*/
/*Fichier : $Id: xzao117.h,v 1.7 2019/05/21 11:06:07 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2019/05/21 11:06:07 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao117.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	11 Oct 1994	: Modification Nb param retournes (v1.1)
* volcic	14 Oct 1994	: Modification Type XDY_FonctionInt (v1.2)
* JMG		07/05/19	: ajout type GAPLA DEM1334 1.3
* GGY		07/09/23 : Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
------------------------------------------------------ */

#ifndef xzao117
#define xzao117

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO117_RPC_NAME "XZAO;117"

#define XZAOC_XZAO117_NB_PARM_RETURN 0

/* definitions de types exportes */
   	
typedef struct {

   	XDY_Eqt			Numero;
   	XDY_NomEqt		ServeurTerminaux;
   	XDY_Octet		LiaisonMaitre;
   	XDY_Octet		LiaisonEsclave;
   	XDY_Rgs			Rgs;
   	XDY_AdresseIP	AdresseIpEsclave;
	XDY_PortIP		PortIpEsclave;
	XDY_AdresseIP	AdresseIpMaitre;
	XDY_PortIP 		PortIpMaitre;

} XZAOT_PIRAU;

typedef struct {

   	XDY_Eqt		Numero;
   	XDY_CodeEqt	CodePAU;
   	XDY_CodeEqt	CodeAntenne;
	XDY_Octet	Type;
   	
} XZAOT_PAU;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO117_Lire_SystemeRAU (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XZAOT_PIRAU *,
					XDY_Entier *);

#endif

