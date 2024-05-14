/*X*/
/* Fichier : $Id: esrv_cfg.h,v 1.8 1996/02/06 17:06:06 volcic Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/06 17:06:06 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESRV * FICHIER esrv_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de configuration de 
*  la tache TESRV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Mismer.D	version 1.2	29 Nov 1994	:
* Mismer.D	version 1.3	13 Dec 1994	:
* Mismer.D	version 1.4	25 Jan 1995	:
* Mismer.D	version 1.5	09 Fev 1995	:
* Fontaine.C	version 1.6	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.7	06 Jun 1995	: Modif type ESRV_EQUIPEMENT
* Mismer.D  	version 1.8	06 Fev 1995	: Modif pour renvoie etat serveur de terminaux
*
------------------------------------------------------ */

#ifndef esrv_cfg
#define esrv_cfg

/* fichiers inclus */

#include "esrv_ctl.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

typedef struct
{
	char	NumeroST[16];	/* Identification du serveur de terminaux (adresse) */
	char	NumLigne[16];	/* Identification de la ligne */
	char	Protocole;	/* Type de protocole ( L, A, O, S ) */
	char 	Sens;		/* Mode de la liaison  ( M : maitre, E : esclave ) */

} ESRV_EQT_LIGNE ;



typedef struct
{
	XZEXT_ENG_PRESENCE_EQT	Eqt;
	ESRV_EQT_LIGNE		LigneEqt[XZECC_MAX_EQT];
	XDY_Mot			Etat;

} ESRV_EQUIPEMENT ;


/* definition de macro exportees */

/* declarations de donnees globales */

extern int		vg_InitTranserver;

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Initialisation du sous-systeme ou une reconfiguration 
*  des 흎uipements.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_init_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel휿 lors de la r휽eption du message
*  XDM_EFIN_CONFIG  

* FONCTION 
*
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la configuration de l'application 
*  la machine locale.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_config	( int va_TypeEquipt );

/*
* ARGUMENTS EN ENTREE :
*   va_TypeEqupt	: type d'흎uipement ( un ou tous )
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Commande des taches equipements ( arret ou lancement tache equipement )
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_cmd_cb ( 
       			T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG);
 

/*
* ARGUMENTS EN ENTREE :
*  Message RTWorks
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*  Fonction appel휿 lors de la r휽eption du message
*  XDM_ECMD_EQPT
*
* FONCTION 
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoie a tsetat le status des serveurs de terminaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void    ec_dde_etat_ST_cb (
                        T_IPC_CONN                      va_Cnx,
                        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
                        T_CB_ARG                        pa_ARG);


/*
* ARGUMENTS EN ENTREE :
*  Message RTWorks
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*  Fonction appel휿 lors de la r휽eption du message
*  XDM_TSE_DIFE
*
* FONCTION

*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Presence_Equip_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_config_eqt	( XDY_NomMachine		pa_NomMachine,
			  int				*pa_NbEqt );

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   pa_NbEqt		: Nombre d'equipement a configurer
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/

         
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Lignes_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_config_lignes	( XDY_NomMachine		pa_NomMachine,
				  int				*pa_NbLigne );

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   pa_NbLigne		: Nombre de ligne a configurer
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/


#endif
