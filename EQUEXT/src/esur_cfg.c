/*E*/
/* Fichier : $Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $        Release : $Revision: 1.28 $        Date : $Date: 2020/11/03 18:12:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESUR * FICHIER esur_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'initialisation et de configuration du sous-systeme
*   EQUEXT pour la tache TESUR.
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Mismer.D	version 1.2	25 Jan 1995	:
* Mismer.D	version 1.3	25 Jan 1995	:
* Mismer.D	version 1.4	26 Jan 1995	:
* Mismer.D	version 1.5	26 Jan 1995	:
* Mismer.D	version 1.6	28 Jan 1995	:
* Mismer.D	version 1.7	09 Fev 1995	:
* Mismer.D	version 1.8	10 Fev 1995	:
* Mismer.D	version 1.9	15 Fev 1995	:
* Mismer.D	version 1.10 	14 Avr 1995	:
* Mismer.D	version 1.11	14 Avr 1995	:
* Mismer.D	version 1.12	26 Avr 1995	:
* Fontaine.C	version 1.13	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.14	19 Jun 1995	: Modif surveillance LT voisin
* Mismer.D	version 1.15	04 Jul 1995	: Modif arret tache
* Mismer.D	version 1.16	20 Sep 1995	: Ajout arret tache tetdc si RADT present 
* Mismer.D	version 1.17	10 Oct 1995	: Ajout lancement tache tetdc si RADT present 
* Mismer.D      version 1.18    31 Oct 1995     : Ajout appel fonction ed_verif_voisin
* Mismer.D      version 1.19    05 Oct 1995     : Modif redemande config apres echec
* Mismer.D  	version 1.20	06 Fev 1995	: Modif pour renvoie etat serveur de terminaux
* Mismer.D  	version 1.21	08 Fev 1995	: Correction modif pour renvoie etat serveur de terminaux (DEM/781)
* Mismer.D  	version 1.22	13 Mar 1996	: Correction modif pour lancer tache protocole operator (DEM/1100)
* Mismer.D 	version 1.23	30 Avr 1996 	: Ajout traitement LT isol� (DEM/1052)
* ???		version 1.24	??/??/??	: changement 'TipcSrvDgGetRecv' en 'TipcSrvSubjectGetSubscribe'
* JMG		version 1.25	12/12/10	: portage linux
* JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : XZIA06 : utiliser un champ d'union  1.26
* JMG		06/09/16 : st 40 ports 1.27
* JMG		17/03/20 : EMS 1.28
* GR            10/07/20 : EMS 1.29
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include 	"xzia.h"

#include 	"ex_mdon.h"

#include 	"esur_art.h"
#include 	"esur_cfg.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : esur_cfg" ;
 	
 	
XZEXT_ENG_CONF_LIGNE	vg_TableLigne[XZECC_MAX_EQT];
ESRV_EQUIPEMENT		vg_TableEqt[XZECC_MAX_EQT];
static char 		vm_ListeST[16][16] 	= { "","","","","","","","","","","","","","","","" };
static int  		vm_EtatST[16] 	= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
extern int		vg_PresenceVoisin;
extern int		vg_RepriseEnCours;
extern int              vg_ConfigTerm;

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int ed_verif_voisin();
extern void ed_arreter_tache_eqpt();


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Initialisation du sous-systeme ou une reconfiguration 
*  des �quipements.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_init_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
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
*  Fonction appel�e lors de la r�ception du message
*  XDM_EFIN_CONFIG  

* FONCTION 
*
*
------------------------------------------------------*/
{
static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : ec_init_cb" ;

#ifdef _TIBCO_EMS
   XDY_Entier 			vl_TypeEqupt;		/* type d'equipement (un ou tous) */ 
   XDY_Entier 			vl_Status;		/* status d'init ou de conf */
#else
   T_INT4 			vl_TypeEqupt;		/* type d'equipement (un ou tous) */ 
   T_INT4 			vl_Status;		/* status d'init ou de conf */
#endif
   XDY_NomSite			pl_NomSite;
   XDY_NomMachine		pl_NomMachine;
   XDY_Datagroup		pl_NomDG;
   int				vl_Fd;
   char 			pl_PathEtFicEqt[XDC_PATH_ABS_SIZE] = "",
   				pl_PathEtFicLig[XDC_PATH_ABS_SIZE] = "";
   int				vl_NbLignes;
   double			vl_Horodate;
   int           		vl_jourSemaine;
   XZEXT_MSG_SOCKET		pl_MsgTimer;
   XDY_Texte			vl_MsgOperateur;
   XZIAT_destinataire		vl_Destinataire;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: entree ec_init_cb");
		  
   /*A Recuperation du status d'execution du message XDM_EFIN_CONFIG */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT4, &vl_TypeEqupt,
               T_IPC_FT_INT4, &vl_Status,
	       NULL); 
   
   vg_RepriseEnCours = XDC_OK;  
   /* Si la machine voisine est OK : return */
   if ( (vg_PresenceVoisin == 0) || (vg_ConfigTerm == XDC_OK) ) return;
	       
   /*A Recuperation du site de la machine */
   if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
   {
      /* Ecriture Trace */
     XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible . " );
   } 
   strcpy ( vl_Destinataire.site, pl_NomSite);
   
   /*A Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
   } 

   /*A Appel de la fonction ec_config du module MCFG pour la configuration des  taches */
   XZST_03EcritureTrace(XZSTC_WARNING,"Debut lancement de l'application. " );
   sprintf(vl_MsgOperateur,"/produits/migrazur/appliSD/exec/ConfigSTTelnet.sh %s",vg_NomMachine);
   system(vl_MsgOperateur);
   if ( ec_config ( vl_TypeEqupt ) != XDC_OK )
   {
      /*A Ecriture trace Configuration de l'application non effectuee */
      XZST_03EcritureTrace(XZSTC_WARNING,"Configuration de l'application non effectuee. " );
      vg_ConfigTerm = XDC_NOK;         
   }
   else
   {
      /*A Envoie message reprise par LT voisin a l'operateur (XZIA06) */
      vg_ConfigTerm = XDC_OK;         
      sprintf ( vl_MsgOperateur, "Toutes les fonctions de %s sont operationnelles sur  %s .\0", vg_NomMachine, pl_NomMachine );
      XZIA_06AffMsgSyst ( vl_Destinataire, vl_MsgOperateur );
      XZST_03EcritureTrace(XZSTC_WARNING,"Application lancee avec succes " );
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: sortie ec_init_cb");

   return; 
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la configuration de l'application 
*  la machine locale.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_config	( int va_TypeEquipt )

/*
* ARGUMENTS EN ENTREE :
*   va_TypeEqupt	: type d'�quipement ( un ou tous )
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
{
   static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : ec_config" ;
   int				vl_ValRet = XDC_OK;
   XDY_NomTache			pl_NomTache;
   XDY_Datagroup		vl_datagroup;
   XZEXT_MASK *			pl_masque;
   char 			pl_PathEtFicEqt[XDC_PATH_ABS_SIZE] = "",
   				pl_PathEtFicLig[XDC_PATH_ABS_SIZE] = "";
   char				pl_PrefTache[5];
   int				vl_NbEqt 	= 0,
   				vl_NbConfLigne 	= 0,
   				vl_IndexEqt 	= 0;
   XZEXT_MASK *   		pl_masqueST = NULL;
   XZEXT_MSG_SOCKET		vl_Command;
   char 			vl_NomST[16]; 
   int				vl_SockRet = 0, vl_NumLigne = 0;
   int				vl_Index, vl_IndexLig;	
   int				vl_nbTestST = 0;
   T_BOOL			vl_EtatDg = FALSE;
				
      
   /*A Test si les fichier de config ligne et presence equipement sont presents */
   sprintf ( pl_PathEtFicEqt, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Presence_Equip, vg_NomMachine );
   sprintf ( pl_PathEtFicLig, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Lignes, vg_NomMachine );
   if ( access(pl_PathEtFicEqt, F_OK|R_OK) && access(pl_PathEtFicLig, F_OK|R_OK) )
   {
       /* Trace fichiers absent et retourne a l'appelant */
      XZST_03EcritureTrace( XZSTC_WARNING, "Fichier de config ligne <%s> \n ou equipement absents <%s>. ", pl_PathEtFicLig, pl_PathEtFicEqt );
      return (XDC_NOK);
   }
       
   /*A 
    * Phase d'initialisation 
    */
      


   /*A
    * Lecture du fichier de configuration XDF_Config_Lignes_<NomMachine> 
    */

   if ( ex_lire_config_lignes ( vg_NomMachine, &vl_NbConfLigne ) != XDC_OK )
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Absence Fichier XDF_Config_Lignes_<NomMachine> . ");
      /*A Si pas de config lignes sortie sans lancement de la configuration */
      return ( XDC_NOK );   
   }

   /*A Si fichier XDF_Config_Lignes_<NomMachine> est vide */     
   if ( vl_NbConfLigne == 0 )
   {
      /*A Ecriture trace fichier vide */
      XZST_03EcritureTrace(XZSTC_WARNING,"Fichier XDF_Config_Lignes_<NomMachine> vide. " );
      return ( XDC_NOK );
   }

   /*A
    * Lecture du fichier de configuration XDF_Presence_Equip_<NomMachine> 
    */
   if ( ex_lire_config_eqt ( vg_NomMachine, &vl_NbEqt ) != XDC_OK )
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Absence Fichier XDF_Presence_Equip_<NomMachine> . ");
      /*A Sortie sans lancement de la configuration */
      return ( XDC_NOK );   
   }

   /*A Si fichier XDF_Presence_Equip_<NomMachine> est vide */     
   if ( vl_NbEqt == 0 )
   {
      /*A Ecriture trace fichier vide */
      XZST_03EcritureTrace(XZSTC_WARNING,"Fichier XDF_Presence_Equip_<NomMachine> vide. " );
      return ( XDC_NOK );
   }
      
   /*A Si configuration  de tous les equipements */
   if ( va_TypeEquipt == XZECC_TYPE_EQT_ALL )
   {
      /*A
       * Arreter toutes les taches traitement et protocole 
       */
       ed_arreter_tache_eqpt ();
 
         
      /*
       *A Test si le serveur de terminaux est accessible 
       */
       if (ex_st40(vg_NomMachine)==XDC_NOK) {
       for ( vl_IndexLig = 0; vl_IndexLig < vl_NbConfLigne; vl_IndexLig++ )
       {
          if ( ( strlen (vg_TableLigne[vl_IndexLig].NumeroST) > 0 ) &&
               ( (vl_NumLigne = atoi (vg_TableLigne[vl_IndexLig].NumLigne) - 1) >= 0 ) &&
               ( (vg_TableLigne[vl_IndexLig].Protocole == XZECC_PROT_LCR) ||
                 (vg_TableLigne[vl_IndexLig].Protocole == XZECC_PROT_TEL) ||
                 (vg_TableLigne[vl_IndexLig].Protocole == XZECC_PROT_OPE) ) )
          {
             if ( strcmp(vl_NomST, vg_TableLigne[vl_IndexLig].NumeroST) )
             {
                if ( vl_SockRet != 0 )
                {
                   ex_fin_cnx ( vl_SockRet, &pl_masqueST);
                   vl_SockRet = 0;
                }
                strcpy ( vl_NomST, vg_TableLigne[vl_IndexLig].NumeroST );
	        while  ( ex_ouvrir_console_ST( vg_TableLigne[vl_IndexLig].NumeroST, &vl_SockRet, &pl_masqueST , vg_NomMachine) != XDC_OK )
	        {
	           sleep (10);
	           if ( vl_nbTestST++ > 2 ) return (XDC_NOK);
	           if ( ed_verif_voisin () != 0 ) return (XDC_NOK);
	        }
	     }
         }
      }
      
      /* Fermeture de la connexion avec le serveur de terminaux */
      if ( vl_SockRet != 0 )
      {
         ex_fin_cnx ( vl_SockRet, &pl_masqueST);
         vl_SockRet = 0;
      }
     	} 

      if ( ed_verif_voisin () != 0 ) return (XDC_NOK);
      /*
       *A Lancement des taches protocoles
       */
      /*A Boucle sur tout les Lignes pour lancer les taches protocoles correspondantes */
      for ( vl_IndexLig = 0; vl_IndexLig < vl_NbConfLigne; vl_IndexLig++ )
      {
         /*A Appel de la fonction el_lancer_tache_protocole */
         if ( (vl_NumLigne = atoi (vg_TableLigne[vl_IndexLig].NumLigne)) != 0 )
         {
            el_lancer_tache_protocole ( vg_TableLigne[vl_IndexLig].Protocole,
            				vg_TableLigne[vl_IndexLig].Sens,
         				vg_TableLigne[vl_IndexLig].NumLigne );
            sleep (10);
         }
      }
         
      /*A Boucle sur tout les equipement pour lancer les taches traitement correspondantes */
      for ( vl_IndexEqt = 0; strlen (vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++ )
      {
         /*A Si l'equipement est present */
         if ( (vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT) &&
              (vg_TableEqt[vl_IndexEqt].Eqt.Type !=  XZECC_TYPE_EQT_SAT) &&
              (vg_TableEqt[vl_IndexEqt].Eqt.Type !=  XZECC_TYPE_EQT_NIC) )
         {
            /* Si tache RADT presente alors lancer tache tetdc.x ( Equation d'alerte RADT ) */
            if ( vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_RDT )
            {
               el_lancer_tache_eqt ( XZECC_TACHE_TDC );
               sleep ( 2 );
            }
            /*A Creation des taches traitement avec un appel a el_lancer_tache_eqt */
            el_lancer_tache_eqt ( vg_TableEqt[vl_IndexEqt].Eqt.Equipement );
         }
      }      
      
      /*A Joindre le DataGroup XDG_ECTL_EQPT_<NomMachine> si pas deja abonne */
      XDG_EncodeDG2 ( vl_datagroup, XDG_ECTL_EQPT, vg_NomMachine) ;
      TipcSrvSubjectGetSubscribe ( vl_datagroup, &vl_EtatDg );
      if ( !vl_EtatDg )
      {
         if ( !XDG_JoindreDG ( vl_datagroup ) )
         {   
            /*A Ecriture trace */         
            XZST_03EcritureTrace( XZSTC_WARNING, " Impossible de joindre le datagroup %s. ", vl_datagroup );
         }
      }
         

   }
    
   return ( vl_ValRet );

}








/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Commande des taches equipements ( arret ou lancement tache equipement )
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_cmd_cb ( 
       			T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG)
 

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
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_EQPT
*
* FONCTION 

*
------------------------------------------------------*/
{
   static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : ec_cmd_cb" ;

    #ifndef _TIBCO_EMS
   T_STR			pl_NomMachineRecp;
   T_INT4 			vl_TypeEqt;		/* type d'equipement (un ou tous) */ 
   T_INT4 			vl_Commande;		/* status d'init ou de conf */
   T_INT4			vl_NumEqt;		/* Numero de l'equipement */
    #else
    XDY_NomMachine		pl_NomMachineRecp;
   XDY_Entier 			vl_TypeEqt;		/* type d'equipement (un ou tous) */ 
   XDY_Entier 			vl_Commande;		/* status d'init ou de conf */
   XDY_Entier			vl_NumEqt;		/* Numero de l'equipement */
    #endif    

   XDY_NomSite			pl_NomSite;
   int				vl_ValRet = XDC_OK;
   XDY_NomTache			pl_NomTache;
   char				pl_PrefTache[5];
   XZEXT_MASK *			pl_masque;
   int				vl_IndexLig = 0, vl_IndexEqt = 0, vl_Index = 0;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: entree ec_cmd_cb");
		  
   /*A Recuperation du status d'execution du message XDM_ECMD_EQPT */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT4, &vl_Commande,
               T_IPC_FT_STR,  &pl_NomMachineRecp,
               T_IPC_FT_INT4, &vl_TypeEqt,
               T_IPC_FT_INT4, &vl_NumEqt,
	       NULL); 
	       
   /*A Recuperation du site de la machine */
   if ( XZSC_04NomSite(pl_NomSite) != XDC_OK )
   {
      /* Ecriture Trace */
     XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible . " );
   } 

   /*A Si le nom de la machine recu dans le message est different du nom de la machine de la tache */
   if ( strcmp ( pl_NomMachineRecp, vg_NomMachine ) != 0 )
   {
      /*A Ecriture trace nom machine incorrecte */
      XZST_03EcritureTrace( XZSTC_WARNING, "ec_cmd_cb : Nom Machine incorrect . " );
      XZST_03EcritureTrace( XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
      return;
   }
   else
   {
      /*A Construction du nom de la tache a partir du type d'equipement */
      if ( ex_nom_eqt ( vl_TypeEqt, pl_PrefTache ) != XDC_OK )
      {
         /*A retourne echec  */
	 return;
      }

      for ( vl_IndexEqt = 0; strlen (vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++ )
      {
         /*A Si l'equipement est present */
         if ( ( vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT ) &&
              ( vg_TableEqt[vl_IndexEqt].Eqt.Type == vl_TypeEqt ) )
            vg_TableEqt[vl_IndexEqt].Etat = vl_Commande;
      }
      
      sprintf ( pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, pl_PrefTache );
	
      /*A 
       * Recherche si la tache existe ( scrutation de la liste des sockets ) 
       */
	 
      /*A Pour toute les sockets du masque */
      for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
      {
         /*A Si differente de la socket RTserver */
         if ( ( pl_masque->desc_sock != vg_SockRT ) && ( pl_masque->desc_sock != vg_SockEct ) )
         {
            /*A Si l'adresse de la socket correspont au nom de la tache */
            if ( strstr ( pl_masque->adresse, pl_NomTache ) != NULL )
            {
               /*A Si demmande d'arret */
               if ( vl_Commande == XZECC_EQT_ARRET )
               {
                  /*A Appel de la fonction ee_config pour emission de la demande d'arret */
                  ee_config ( XZEXC_DEMANDE_ARRET, pl_masque->desc_sock, pl_masque->adresse );
      		  XZST_03EcritureTrace( XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
      	          return;
               }
               else
               /*A Si demmande de lancement de tache */
               {
      		  XZST_03EcritureTrace( XZSTC_WARNING, "ec_cmd_cb : Tache %s deja lanc�e . ", pl_NomTache );
      		  XZST_03EcritureTrace( XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
      	          return;
               }
            }	     
         }   
      }   /* fin for */	 
   
      
      /*A
       * La tache demandee n'est pas en cours d'execution 
       */      
       
      /*A Si demmande d'arret */
      if ( vl_Commande == XZECC_EQT_ARRET )
      {
         /*A Sortie erreur */
      	 XZST_03EcritureTrace( XZSTC_WARNING, "ec_cmd_cb : Tache %s n'est pas en cours . ", pl_NomTache );
      	 XZST_03EcritureTrace( XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
      	 return;
      }

      /*A Si demmande de lancement de tache */

      /*A Boucle sur tout les equipement pour lancer les taches traitement correspondantes */
      for ( vl_IndexEqt = 0; strlen (vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++ )
      {
         /*A Si l'equipement est present */
         if ( ( vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT ) &&
              ( vg_TableEqt[vl_IndexEqt].Eqt.Type ==  vl_TypeEqt ) &&
              ( vg_TableEqt[vl_IndexEqt].Eqt.Type !=  XZECC_TYPE_EQT_SAT) &&
              ( vg_TableEqt[vl_IndexEqt].Eqt.Type !=  XZECC_TYPE_EQT_NIC) )
         {
            /*A lancement des taches protocoles */
            for ( vl_Index = 0;
                  strlen(vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].NumLigne) != 0;
                  vl_Index++ )
            {
               el_lancer_tache_protocole ( vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].Protocole,
            				   vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].Sens,
            				   vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].NumLigne );
               sleep (10);
            }   
            
            /* Si tache RADT presente alors lancer tache tetdc.x ( Equation d'alerte RADT ) */
            if ( vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_RDT )
            {
               el_lancer_tache_eqt ( XZECC_TACHE_TDC );
               sleep ( 2 );
            }
            /*A Creation des taches traitement avec un appel a el_lancer_tache_eqt */
            el_lancer_tache_eqt ( vg_TableEqt[vl_IndexEqt].Eqt.Equipement );
          return;
         }
      }      
   }
   
   XZST_03EcritureTrace( XZSTC_WARNING, " Tache traitement %s inconnue. ",
          	    			 vg_TableEqt[vl_IndexEqt].Eqt.Equipement ); 
   return;
   
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Presence_Equip_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_config_eqt	( XDY_NomMachine		pa_NomMachine,
			  int				*pa_NbEqt )

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
{
   static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : ex_lire_config_eqt" ;
   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   char				*pl_Donnee;
   XDY_Commentaire		pl_LigneFich;
   int				vl_NbEqt = 0, vl_Presence = 0, vl_Index = 0;
   

      /*A Ouverture du fichier presence equipement */
      /* Construction du nom du fichier XDF_Presence_Equip_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Presence_Equip, pa_NomMachine );
      
      /*A Ouverture du fichier XDF_Presence_Equip_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /*A Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. ", pl_PathEtFic ); 
         /*A Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /*A Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*A Extraire le nom de l'equipement et si il est present */
             sscanf ( pl_LigneFich, 	"%s %d %d", 
             				vg_TableEqt[vl_NbEqt].Eqt.Equipement,
             				&vl_Presence,
             				&vg_TableEqt[vl_NbEqt].Eqt.Type );
             vg_TableEqt[vl_NbEqt].Eqt.Present = vl_Presence;
             XZST_03EcritureTrace( XZSTC_DEBUG1, "Equipt:%s Pres:%d type:%d",
             				vg_TableEqt[vl_NbEqt].Eqt.Equipement,
             				vg_TableEqt[vl_NbEqt].Eqt.Present,
             				vg_TableEqt[vl_NbEqt].Eqt.Type );
             pl_Donnee = strtok ( pl_LigneFich, "$" );
             while ( (pl_Donnee = strtok ( NULL, "$" )) != NULL )
             {
               XZST_03EcritureTrace( XZSTC_DEBUG1, "Param:%s",pl_Donnee);
               if ( sscanf ( pl_Donnee, " %s %s %c %c",
                			vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].NumeroST,
                			vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].NumLigne,
                			&vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].Protocole,
                			&vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].Sens ) == 4 )
                {
                   XZST_03EcritureTrace( XZSTC_DEBUG1, "ST:%s Lig:%s Prot:%c Sens:%c",
                			vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].NumeroST,
                			vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].NumLigne,
                			vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].Protocole,
                			vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].Sens );
                   vl_Index++;
                }
             }
               			
             XZST_03EcritureTrace( XZSTC_DEBUG1, " Eqt : %s      Presence : %d \n", vg_TableEqt[vl_NbEqt].Eqt.Equipement, vg_TableEqt[vl_NbEqt].Eqt.Present);
             /*A Si ligne lu non vide */
             if ( strlen ( vg_TableEqt[vl_NbEqt].Eqt.Equipement ) > 0 )
             {
                 /*A Incrementer le pointeur sur le tableau */
                 vl_NbEqt++;
             }
         }
      }
      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      /*A Renvoie le nombre d'equipement a configurer */
      *pa_NbEqt = vl_NbEqt;
      /*A Retourne compte rendu OK */
      return ( vl_ValRet );
}


         
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Lignes_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_config_lignes	( XDY_NomMachine		pa_NomMachine,
				  int				*pa_NbLigne )

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
{
   static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : ex_lire_config_lignes" ;
   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Commentaire		pl_LigneFich;
   int				vl_NbConfLigne = 0;
   int				vl_NbST	= 0;
   

      /*A
       * Lecture du fichier de configuration XDF_Config_Lignes_<NomMachine> 
       */
      /*A Construction du nom du fichier XDF_Config_Lignes_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Lignes, pa_NomMachine );
      
      /*A Ouverture du fichier XDF_Config_Lignes_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /*A Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. ", pl_PathEtFic ); 
         /*A Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /*A Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*A Extraire le nom de l'equipement et l'option presence */
            sscanf ( pl_LigneFich, "%s %s %c %c",
              			     vg_TableLigne[vl_NbConfLigne].NumeroST,
             			     vg_TableLigne[vl_NbConfLigne].NumLigne,
             			     &(vg_TableLigne[vl_NbConfLigne].Protocole),
                              	     &(vg_TableLigne[vl_NbConfLigne].Sens) );
                    		     
              XZST_03EcritureTrace ( XZSTC_DEBUG1, "ST:%s/NL:%s/P:%c/S:%c",   
             			     vg_TableLigne[vl_NbConfLigne].NumeroST,
             			     vg_TableLigne[vl_NbConfLigne].NumLigne,
             			     vg_TableLigne[vl_NbConfLigne].Protocole,
             			     vg_TableLigne[vl_NbConfLigne].Sens );
             /*A Si ligne lu non vide */
             if ( strlen ( vg_TableLigne[vl_NbConfLigne].NumeroST ) > 0 )
             {
                 /*B Incrementer le nombre d'equipement */
                 if ( strcmp(vm_ListeST[vl_NbST], vg_TableLigne[vl_NbConfLigne].NumeroST) )
                 {
                    vl_NbST += (vm_ListeST[vl_NbST][0] == '\0') ? 0 : 1;
                    strcpy ( vm_ListeST[vl_NbST], vg_TableLigne[vl_NbConfLigne].NumeroST );
                 }
                 vl_NbConfLigne++;
             }
         }
      }
      /*A Fermeture du fichier  XDF_Config_Lignes_<NomMachine> */
      XZSS_13FermerFichier( vl_Fd );
      /*A Renvoie le nombre de ligne a configurer */
      *pa_NbLigne = vl_NbConfLigne;
      strcpy ( vm_ListeST[++vl_NbST], "" );
      /*A Retourne compte rendu OK */
      return ( vl_ValRet );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_verif_arret_tache	( void )

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
{
         int            vl_IndexEqt = 0;
         int            vl_IndexLig = 0;

         /*
          *A Verification arret des taches protocoles
          */
         /*A Boucle sur tout les Lignes pour arret les taches protocoles correspondantes */
         for ( vl_IndexLig = 0; vl_IndexLig < XZECC_MAX_EQT; vl_IndexLig++ )
         {
            /*A Appel de la fonction el_lancer_tache_protocole */
            if ( atoi (vg_TableLigne[vl_IndexLig].NumLigne) != 0 )
               el_arret_tache ( vg_TableLigne[vl_IndexLig].Protocole,
                                vg_TableLigne[vl_IndexLig].Sens,
                                vg_TableLigne[vl_IndexLig].NumLigne );
         }
      
         /*A Boucle sur tout les equipement pour arreter les taches traitement correspondantes */
         for ( vl_IndexEqt = 0; strlen (vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++ )
         {
            /*A Si l'equipement est present */
            if ( vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT )
            {
               if ( vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_RDT )
                  el_arret_tache ( 0, 0, XZECC_TACHE_TDC ) ; 
               el_arret_tache ( 0, 0, vg_TableEqt[vl_IndexEqt].Eqt.Equipement ) ; 
            }
         }      


}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Lignes_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int lire_machine_sup	( void )

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
{
   static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : lire_machine_sup" ;
   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Commentaire		pl_LigneFich;
   int				vl_NbConfLigne = 0;
   XDY_NomMachine       	pl_NomMachine;
   

        /*A Recuperation du nom de la machine */
	if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
	{
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
	}
	 
      /*A
       * Lecture du fichier de configuration  XDF_Machine_Sup
       */
      /*A Construction du nom du fichier XDF_Machine_Sup  */
      sprintf ( pl_PathEtFic, "../fichiers/deq/%s_%s", XDF_Machine_Sup, pl_NomMachine );
      
      /*A Ouverture du fichier XDF_Machine_Sup  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /*A Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. ", pl_PathEtFic ); 
         /*A Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /*A Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*A Extraire le nom de l'equipement et l'option presence */
             if ( sscanf ( pl_LigneFich, "%s", vg_NomMachine ) != 1 )
             {
                 XZST_03EcritureTrace( XZSTC_WARNING, " Impossible de lire le nom de la machine supleante " ); 
                 vl_ValRet = XDC_NOK;
             }
             else
             {
                 XZST_03EcritureTrace( XZSTC_INFO, " Nom de la machine supleante:%s", vg_NomMachine ); 
             }
          }
      }
      /*A Fermeture du fichier  XDF_Config_Lignes_<NomMachine> */
      XZSS_13FermerFichier( vl_Fd );
      /*A Retourne compte rendu OK */
      return ( vl_ValRet );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la surveillance des taches de la machine.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_surveillance_tache ( void )

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
{
   static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : ec_surveillance_tache" ;
   XDY_NomTache			pl_NomTache;
   char				pl_PrefTache[5];
   XZEXT_MASK *			pl_masque;
   int				vl_IndexLig = 0, vl_IndexEqt = 0, vl_Index = 0;
   static	int		vl_CptTest = 4;


      /*A Boucle sur tout les equipement pour lancer les taches traitement correspondantes */
      for ( vl_IndexEqt = 0; strlen (vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++ )
      {
         /*A Si l'equipement est present */
         if ( ( vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT ) &&
              ( vg_TableEqt[vl_IndexEqt].Etat !=  XZECC_EQT_ARRET ) &&
              ( vg_TableEqt[vl_IndexEqt].Eqt.Type !=  XZECC_TYPE_EQT_SAT ) &&
              ( vg_TableEqt[vl_IndexEqt].Eqt.Type !=  XZECC_TYPE_EQT_NIC ) )
         {
            if ( ex_nom_eqt ( vg_TableEqt[vl_IndexEqt].Eqt.Type, pl_PrefTache ) == XDC_OK )
            {
               sprintf ( pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, pl_PrefTache );
               /*A  Recherche si la tache existe ( scrutation de la liste des sockets ) */
               for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
                  /*A Si l'adresse de la socket correspont au nom de la tache */
                  if ( strstr ( pl_masque->adresse, pl_NomTache ) != NULL ) break;
               if ( pl_masque == NULL )
               {
                  /*A lancement des taches protocoles */
                  for ( vl_Index = 0; vl_Index < XZECC_MAX_EQT; vl_Index++ )
                  {
                     if ( strlen(vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].NumLigne) != 0 )
                     {
                        el_lancer_tache_protocole ( vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].Protocole,
            		      		            vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].Sens,
            				            vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].NumLigne );
                        sleep (10);
                     }
                  }   
            
	          /* Si tache RADT presente alors lancer tache tetdc.x ( Equation d'alerte RADT ) */ 
	          if ( vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_RDT )
	          {
	             el_lancer_tache_eqt ( XZECC_TACHE_TDC ); 
	             sleep ( 2 ); 
	          }

                  /*A Creation des taches traitement avec un appel a el_lancer_tache_eqt */
                  el_lancer_tache_eqt ( vg_TableEqt[vl_IndexEqt].Eqt.Equipement );
               }
            }
         }
      }     
      
      /*A Test etat serveur de terminaux */    
      if ( vl_CptTest++ > 3 )
      {
         vl_CptTest = 0;
         for ( vl_Index = 0; vm_ListeST[vl_Index][0] != '\0' ; vl_Index++ )
         {
            if (  ex_presence_machine ( vm_ListeST[vl_Index] ) == XDC_OK )
            {
               if ( vm_EtatST[vl_Index] != XZSEC_ETAT_OK )
               {
                  vm_EtatST[vl_Index] = XZSEC_ETAT_OK;
                  XZSE_14EnvoieEtatPeriph ( vm_ListeST[vl_Index], vm_EtatST[vl_Index] );
                  XZST_03EcritureTrace(XZSTC_WARNING,"Transerver %s OK", vm_ListeST[vl_Index] );
               }
            }
            else
            {
               if ( vm_EtatST[vl_Index] != XZSEC_ETAT_NOK )
               {
                  vm_EtatST[vl_Index] = XZSEC_ETAT_NOK;
                  XZSE_14EnvoieEtatPeriph ( vm_ListeST[vl_Index], vm_EtatST[vl_Index] );
                  XZST_03EcritureTrace(XZSTC_WARNING,"Transerver %s NOK", vm_ListeST[vl_Index] );
               }
            }
         }
      }

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoie a tsetat le status des serveurs de terminaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_dde_etat_ST_cb ( 
       			T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG)
 

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
*  Fonction appel�e lors de la r�ception du message
*  XDM_TSE_DIFE
*
* FONCTION 

*
------------------------------------------------------*/
{
   static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : ec_dde_etat_ST_cb" ;
   int 		vl_Index = 0;
   T_IPC_MT	vl_Mt ;
#ifdef _TIBCO_EMS
   XDY_Entier  vl_Num_Msg = 0;
#else
   T_INT4      vl_Num_Msg = 0;
#endif
    
   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: IN  ec_dde_etat_ST_cb");
   
#ifndef _TIBCO_EMS
   if ( (TipcMsgGetType(pa_Data->msg,&vl_Mt)) && 
        (TipcMtGetNum(vl_Mt,&vl_Num_Msg)) && 
        (vl_Num_Msg == XDM_TSE_DIFE) )
#else
     if ( (TipcMsgGetType(pa_Data->msg,(int *)&vl_Num_Msg)) &&
             (vl_Num_Msg == XDM_TSE_DIFE) )
#endif
   {
      /*A Pour tous les serveurs de terminaux */    
      for ( vl_Index = 0; vm_ListeST[vl_Index][0] != '\0' ; vl_Index++ )
         /*A Appel de XZSE_14EnvoieEtatPeriph */    
         XZSE_14EnvoieEtatPeriph ( vm_ListeST[vl_Index], vm_EtatST[vl_Index] );
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: OUT ec_dde_etat_ST_cb Numero de Msg:%d", vl_Num_Msg );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoie a tsetat le status des serveurs de terminaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *	ec_Nom_ST ( void )
 

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
{
   static char *version = "$Id: esur_cfg.c,v 1.28 2020/11/03 18:12:33 pc2dpdy Exp $ : ec_Nom_ST" ;
   int 		vl_Index = 0;
   T_IPC_MT	vl_Mt ;
   T_INT4       vl_Num_Msg = 0;
    
   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: IN  ec_Nom_ST");
   
   for ( vl_Index = 0; vm_ListeST[vl_Index][0] != '\0' ; vl_Index++ )
     if ( vm_ListeST[vl_Index][0] != '\0' )
         break;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: OUT ec_Nom_ST : %s ", vm_ListeST[vl_Index] );
   return ( vm_ListeST[vl_Index] );
}

