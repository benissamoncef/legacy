/*E*/
/*Fichier : $Id: xzao722.h,v 1.1 2020/04/02 09:04:22 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2020/04/02 09:04:22 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao722.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMg	06/01/2020 creation 1.1
*
------------------------------------------------------ */

#ifndef xzao722
#define xzao722

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO722_RPC_NAME "XZAO722"

#define XZAOC_XZAO722_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		Numero;
	XDY_Octet	TypeIMU;
	XDY_District	SiteGestion;	
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP	Port;
                        char            Namespace[50];
                        char            Protocole[20];
                        char            Servername[20];
                        char            Nodeprefix[20];

} XZAOT_IMU;


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO722_Liste_IMU_Machine (	XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
