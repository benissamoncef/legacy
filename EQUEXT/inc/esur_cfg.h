/*X*/
/* Fichier : $Id: esur_cfg.h,v 1.6 2011/02/24 17:43:28 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2011/02/24 17:43:28 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESUR * FICHIER esur_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de configuration de la tache TESUR
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Fontaine.C	version 1.2	22 Mai 1995	: Reprise entete
* Mismer.D	version 1.3	13 Oct 1994	: Ajout parametre Etat pour relance auto des taches
* Mismer.D  	version 1.4	06 Fev 1995	: Modif pour renvoie etat serveur de terminaux
* Mismer.D 	version 1.5	30 Avr 1996 	: Ajout traitement LT isol� (DEM/1052)
* JPL		24/02/11 : Migration HP ia64 (DEM 975) : declaration ec_surveillance_tache, ec_verif_arret_tache  1.6
------------------------------------------------------ */

#ifndef esur_cfg
#define esur_cfg

/* fichiers inclus */

#include "esur_ctl.h"

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
        XDY_Mot                 Etat;

} ESRV_EQUIPEMENT ;


/* definition de macro exportees */

/* declarations de donnees globales */
extern XDY_NomMachine vg_NomMachine;

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


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoie a tsetat le status des serveurs de terminaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *	ec_Nom_ST ( void );
 

/*
* ARGUMENTS EN ENTREE :
*  Aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Nom du serveur de terminaux associe au LT
*
* CONDITION D'UTILISATION
*
* FONCTION 
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

int lire_machine_sup	( void );

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
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
*  Cette fonction effectue la surveillance des taches de la machine.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_surveillance_tache ( void );

/*
* ARGUMENTS EN ENTREE :
*   aucun
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
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_verif_arret_tache	( void );

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/


#endif
