/*E*/
/* Fichier : ex_mala.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mala.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module commun pour envoi des alarmes 
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	26 Oct 1994	: Creation
* Mismer.D	version 1.2	08 Nov 1994	: 
* Mismer.D	version 1.3	09 Nov 1994	: Ajout memorisation presence alarme par fichier
* Mismer.D	version 1.4	09 Nov 1994	:
* Mismer.D	version 1.5	14 Dec 1994	:
* Mismer.D	version 1.6	06 Jan 1995	:
* Mismer.D	version 1.7	25 Jan 1995	:
* Mismer.D	version 1.8	09 Fev 1995	:
* Volcic.F	version 1.9	20 Mar 1995	: Ajout envoi message Dataviews : Etat des liaisons ds ex_env_alarme
* Mismer.D	version 1.10	20 Mar 1995	: Ajout envoi alarme fugitive si nombre d'erreur > 1
* Volcic.F	version 1.11	10 Mai 1995	:
* Mismer.D	version 1.12	22 Mai 1995	: Modif texte du fichier d'alarme, reprise entete, historique
* Mismer.D	version 1.13	31 Mai 1995	: Modif taille nom equipement 
* Mismer.D	version 1.14	02 Oct 1995	: Modif pour acknoledge alarme phase 1 
* Mismer.D	version 1.15	03 Oct 1995	: Modif pour acknoledge alarme phase 2
* Mismer.D	version 1.16	23 Oct 1995	: Modif pour reemission alarme au lancement d'une tache
* Mismer.D	version 1.17	23 Nov 1995	: Suppression des alarmes OPEN-VIEW pour SD ou SP
* Mismer.D	version 1.18	06 Fev 1996	: Correction datagroup envoi etat liaison
* Mismer.D	version 1.19	07 Fev 1996	: Ajout fonction suppression alarme (pour eqt HS)
* Mismer.D  	version 1.20	02 Sep 1996	: Ajout type machine (RADT_W) 
* Mismer.D  	version 1.21	05 Dec 1996	: Modif pour fichier alerte (DEM/1232) 
* JMG		version 1.22	10 Jan 2004	: Modif constante EALAC_LON_TEXTE_ALARME passe de 100 a 120	
* JMG   13/11/07        : ajout site de gestion DEM715  1.25
* JMG	12/12/10	: portage linux  1.27
* JPL	25/02/11 : Migration HP ia64 (DEM 975) : retour de C.R. OK ou NOK (ex_remonte_al_ovw, ex_env_etat_liaison)  1.28
* JPL	25/02/11 : Migration architecture HP ia64 (DEM 975) : adequation format scanf et type d'entier  1.29
* JMG	25/01/13 : niveaux de traces 1.30
* PNI	16/06/21 : suppression traces dans ex_reemission_alarme 1.31
* JPL	16/11/21 : Modifications mineures (visibilite, versions, traces); identique precedente  1.32
* JPL	17/11/21 : Utilisation minuterie generique: plus de code fonctionnel execute sous interruption a l'echeance (SAE_339)  1.33
* JPL	19/11/21 : Ajout "ex_traiter_alarmes", point d'acces a la fonction principale pour les taches externes (SAE_339)  1.34
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* JPL	01/06/22 : XZSS_22Attendre : argument entier en millisecondes  1.35
------------------------------------------------------*/

/* fichiers inclus */

#include "xzam.h"
#include "xzsa.h"

#include "ex_mala.h"
#include "ex_mlcr.h"
#include "ex_msok.h"

static char *version = "ex_mala.c 1.35" ;

/* definitions de constantes */

#define	EALAC_TAILLE_FIC_ALARME		100000
#define EALAC_LON_TEXTE_ALARME		120
#define EALAC_NB_ESSAI_OPEN		1000
#define EALAC_TIMEOUT_ALARME		150

#define EALAC_MACHINE_OPENVIEW		"POCMA"
#define EALAC_REP_GTC_OPENVIEW		"/produits/OV/interfaces/gtc/fichiers"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

EX_MALAT_ALARME * 	pm_ListeAlarme          = NULL;
EX_MALAT_INFO_ALARME * 	pm_ListeInfoAlarme      = NULL;
EX_MALAT_ALARME * 	pm_ListeAlarmeInit      = NULL;
XZSCT_NomTache    	vm_NomTache    	  	= "";
int		  	vm_InitAlarme		= XDC_NOK;

/* declaration de fonctions internes */
void ex_reemission_alarme ( int );
void ex_acknoledge_alarme     ( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
void ex_abt_acknoledge_alarme ( XDY_NomMachine, XDY_TypeEqt );
void ex_ajout_liste_alarme    ( EX_MALAT_ALARME * );
static void ex_wfic_liste_alarme     ( XDY_NomMachine );
static void ex_rfic_liste_alarme     ( XDY_NomMachine );
int  ex_existe_alarme         ( EX_MALAT_ALARME * );
EX_FONCT_INSERER   ( EX_MALAT_INFO_ALARME, ex_ins_liste_alarme, pm_ListeInfoAlarme, Numero, Suivant, Suivant)

/*******************************************************
* DESCRIPTION DE LA FONCTION :
* Fonction permettant la remontee des alarmes des LT vers OpenView.
* Controle si une precedente alarme est deja en cours. Si oui retour
* a la fonction appelante sinon lancement de la remontee vers OpenView.
*
*******************************************************
* SEQUENCE D'APPEL :
*
* int ex_remonte_al_ovw(va_Etat, pa_NomMachine)
*
*
* PARAMETRES EN ENTREE :
*
* XDY_Booleen     va_Etat       : Etat de l'alarme 
* XDY_NomMachine  pa_NomMachine : Nom de la machine
*
* PARAMETRES EN SORTIE :
*
*
*
* VALEUR RENDUE :
*
* XDC_OK / XDC_NOK
*
* CONDITION D'ERREUR :
* 
* Execution incorrecte du lancement de trap
* ou de la non verification de l'existence de precedente alarme.
*
* CONDITIONS LIMITES :
*
*
*******************************************************/  
static int ex_remonte_al_ovw( va_Etat, pa_NomMachine )
  XDY_Booleen     va_Etat;
  XDY_NomMachine  pa_NomMachine;
{
 FILE        *pl_pipe;          /* Descripteur du canal logique entre le 
                                   processus et la commande a executer */
 char        vl_commande[128];  /* Commande a executer */
 char        vl_tampon[128];    /* Variable temporaire */
 int         vl_Fd 	      = 0;
 char        pl_NomFichier[XDC_PATH_ABS_SIZE];
 XDY_Texte   pl_LigneFich     = "";
 int	     vl_Compteur      = 0;
 static int  vl_TypeMac       = 0;
 XDY_NomMachine  pl_NomMachine;

 /* Sil ne s'agit pas d'un LT alors return */
 if ( !vl_TypeMac )
    if ( XZSA_17TypeMachine ( pa_NomMachine, &vl_TypeMac ) != XDC_OK )
    {
       XZSA_20InitAdmin();
       if ( XZSA_17TypeMachine ( pa_NomMachine, &vl_TypeMac ) != XDC_OK )
          XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de trouve le type de la machine" );
    }
 if ( (vl_TypeMac != XZSCC_TYPEM_LT) && (vl_TypeMac != XZSCC_TYPEM_PCS) ) return ( XDC_OK );

 /*A Ouverture du canal de communication avec la commande "ls -l" */
 sprintf(vl_commande,"ls -l %s | grep Ala_%s",XZEXC_PATH_FIC_ALARME,pa_NomMachine);
 if ((pl_pipe = popen(vl_commande,"r")) == NULL)
 {
    /*A Canal de communication non ouvert -> sortie avec erreur */
    return (XDC_NOK);
 }
 
 /* Lecture du premier elements du pipe pour eliminer le compte rendu de la commande */
 fgets(vl_tampon,128,pl_pipe);
 if ( feof(pl_pipe) != 0)
 {
    pclose ( pl_pipe );
    if  ( ex_presence_machine(EALAC_MACHINE_OPENVIEW) !=  XDC_OK)
    {
       /*A Canal de communication non ouvert -> sortie avec erreur */
        return (XDC_NOK);
    }
    /*A Le repertoire contenant les fichiers des alarmes est vide */
    sprintf ( pl_NomFichier, "/tmp/%s", pa_NomMachine );
    if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) ) < 0 )
    {
       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer le fichier compteur alarme pour OpenView" );
       return (XDC_NOK);
    }

    /*A Lecture de chaque ligne du fichier */
    if ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF )
    {
       /*A Extraire les parametre de l'alarme */
       sscanf  ( pl_LigneFich, "%d", &vl_Compteur );
    }
    sprintf ( pl_LigneFich, "%d\n", ++vl_Compteur );
    if ( !lseek ( vl_Fd, 0, SEEK_SET ) )
    {
       write ( vl_Fd, pl_LigneFich, strlen(pl_LigneFich) );
    }
    close ( vl_Fd );


    sprintf ( pl_NomFichier, "/tmp/%s.%s", pa_NomMachine, (va_Etat == XDC_VRAI) ? "down" : "up");
    if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) ) < 0 )
    {
       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer le fichier UP ou DOWN alarme pour OpenView" );
       return (XDC_NOK);
    }
    write ( vl_Fd, pl_LigneFich, strlen ( pl_LigneFich) );
    close ( vl_Fd );
    if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
    {
       /* Ecriture Trace */
       XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
    }
    sprintf ( pl_NomFichier, "%s.%s", pa_NomMachine, (va_Etat == XDC_VRAI) ? "down" : "up");
    if ( XZSS_07TransfertFichier (    pl_NomMachine,
                                      pl_NomFichier,
                                      "/tmp",
                                      EALAC_MACHINE_OPENVIEW,
                                      pl_NomFichier,
                                      EALAC_REP_GTC_OPENVIEW ) != XDC_OK )
     {
         XZST_03EcritureTrace( XZSTC_WARNING, "Transfert fichier %s de %s vers %s impossible",
                                               pl_NomFichier,
                                               pl_NomMachine,
                                               EALAC_MACHINE_OPENVIEW );
     }
     else
     {
         XZST_03EcritureTrace( XZSTC_INFO, "Transfert fichier %s de %s vers %s OK",
                                            pl_NomFichier,
                                            pl_NomMachine,
                                            EALAC_MACHINE_OPENVIEW );
      }
 }
 else
 {
    pclose ( pl_pipe );
 }
 return (XDC_OK);
      
} /* fin fonction ex_remonte_al_ovw */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Lecture du fichier de config des alarmes.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ex_lecture_fichier_alarme (	XDY_TypeAlarme	va_Type, EX_MALAT_INFO_ALARME * va_InfoAlarme )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*
------------------------------------------------------*/
{
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich = "";
   char *			vl_Texte;
   XDY_Mot			vl_Urgence[3] = { 0, 0, 0 };
   EX_MALAT_INFO_ALARME		*pl_DonCrt = NULL;
   
      for ( pl_DonCrt = pm_ListeInfoAlarme; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
         if ( pl_DonCrt->Numero == va_Type )
         {
            memcpy ( va_InfoAlarme, pl_DonCrt, sizeof ( EX_MALAT_INFO_ALARME ) );
            return ( XDC_OK );
         }
      }
      /*A Ouverture du fichier de config des alarmes */
      /* Construction du nom du fichier de config des alarmes */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_CONF_ALARME  );
      
      /*A Ouverture du fichier de config des alarmes */
      if ( XZSS_12OuvrirFichier( O_RDONLY , pl_PathEtFic, &vl_Fd ) != XDC_OK )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. ", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             
             if ( sscanf ( pl_LigneFich, "%d\t%[^\t]\t%hd\t%hd\t%hd",  
             				&va_InfoAlarme->Numero,
             				va_InfoAlarme->Libelle,
             				&vl_Urgence[0],
             				&vl_Urgence[1],
             				&vl_Urgence[2] ) != 5 )
             {
                XZST_03EcritureTrace( XZSTC_WARNING, "Lecture info d'alarme incorrecte %s:%d",  
             				&va_InfoAlarme->Numero,
             				va_InfoAlarme->Libelle );
                continue; 
             }
             
             if ( va_InfoAlarme->Numero != va_Type ) continue;
             
             if ( vl_Urgence[0] )
                va_InfoAlarme->Urgence = XZAMC_CRITIQUE;
             else va_InfoAlarme->Urgence = ( vl_Urgence[1] ) ? XZAMC_MAJEURE : XZAMC_MINEURE;
             
             ex_ins_liste_alarme ( va_InfoAlarme, 0 );
             break;
         }
      }
      /*A Fermeture du fichier de config des alarmes  */
      XZSS_13FermerFichier( vl_Fd );
      
     
      /* Ecriture trace de la config pour debug */
#ifdef _HPUX_SOURCE 
      for ( pl_DonCrt = pm_ListeInfoAlarme; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG1, "Alarme:  %d %s %d",  
               			 pl_DonCrt->Numero,
				 pl_DonCrt->Libelle,
				 pl_DonCrt->Urgence );				
      }
#endif
      /* Retourne compte rendu OK */
      return ( XDC_OK );

}



/* definition de fonctions externes */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'une alarme vers le sous-systeme BASEDD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_env_alarme  ( 	XDY_NomMachine	pa_NomMachine,
			XDY_Horodate	va_Horodate,
			XDY_TypeEqt	va_TypeEqt,
			XDY_Eqt		va_NumEqt,
			XDY_TypeAlarme	va_Type,
			XDY_Booleen	va_Etat,
			XDY_Texte	va_ArgAlarme,
			XDY_District	va_SiteGestion)


/*
* ARGUMENTS EN ENTREE :
*
*  XDY_NomMachine	pa_NomMachine	: Nom machine
*  XDY_Horodate		va_Horodate	: Horodate
*  XDY_TypeEqt		va_TypeEqt	: Type d'equipement 
*  XDY_Eqt		va_NumEqt	: Numero d'equipement 
*  XDY_TypeAlarme	va_Type		: Numero d'alarme
*  XDY_Booleen		va_Etat		: Etat de l'alarme
*  char *		va_ArgAlarme	: Argument supplementaire pour l'alarme  
*
* ARGUMENTS EN SORTIE
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Controle si l'alarme est en cours ou non et selon le cas retourne a l'appelent ou
*     Envoi de l'alarme a la base  et ecriture dans fichier local machine
*     Si mode isole ou erreur envoi msg alors ecriture du message dans fichier
*
------------------------------------------------------*/
{
static char *fonction = "ex_env_alarme" ;

XDY_Datagroup		pl_NomDg, pl_DG_Liaison;
XDY_NomSite		pl_NomSite = "";
XDY_NomMachine		vl_NomMachine = "";
XDY_Entier		vl_RetCreerAlarme = XDC_OK,
			vl_NouvelleAlarme = XDC_NOK;
XDY_Horodate		vl_Horodate = 0.0;
XZSMT_Horodate       	pl_StrHorodate;
long			vl_PtrFile  = 0;
long			vl_CptEssai = 0;
char 			pl_NomFichier[XDC_PATH_ABS_SIZE];
char			pl_LigneFich[EALAC_LON_TEXTE_ALARME+10], pl_Entete[EALAC_LON_TEXTE_ALARME+10];
char			vl_NomEqt[50] = "";
XDY_NomEqt		vl_NomTypeEqt = "";
size_t			vl_LgTexte = 0;
int			vl_Fd = 0, vl_FD_Ala = 0;
static	char		vl_Entete[] = "#Pointeur dernier enregistrement: %09ld\n";
EX_MALAT_ALARME         vl_Alarme;
EX_MALAT_INFO_ALARME    vl_InfoAlarme;
int                     vl_js;
XDY_Horodate            vl_horodate;

      XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_js , &vl_horodate );
      vl_Alarme.Horodate	= vl_horodate;
      vl_Alarme.TypeEqt		= va_TypeEqt;
      vl_Alarme.NumEqt		= va_NumEqt;
      vl_Alarme.TypeAlarme	= va_Type;
      vl_Alarme.Etat		= va_Etat;
      vl_Alarme.SiteGestion     = va_SiteGestion;
      strcpy ( vl_Alarme.NomMachine, pa_NomMachine );

      ex_nom_eqt ( va_TypeEqt, vl_NomEqt );
      if ( va_TypeEqt == XDC_EQT_PCT )
         strcpy ( vl_NomEqt, "picto" );
      /* Construction du nom du fichier memorisant l'etat de l'alarme */
      sprintf ( pl_NomFichier, "%s/Ala_%s_%s_%03d_%03d",
      				XZEXC_PATH_FIC_ALARME,  
      				pa_NomMachine,
      				vl_NomEqt,
      				va_NumEqt,
      				va_Type );
      				
      /*A Test si nouvelle alarme */
      if ( access ( pl_NomFichier, F_OK ) )
      {
         /* Nouvelle Alarme */
         vl_NouvelleAlarme = XDC_OK;
      }
      
      /* Test s'il s'agit d'une apparition d'alarme */
      if ( ( va_Etat == XDC_VRAI ) && ( vl_NouvelleAlarme == XDC_OK ) )
      {
         /* Appel de la fonction remontee des alarmes a OPEN VIEW */
         ex_remonte_al_ovw ( XDC_VRAI, pa_NomMachine );
         /* Creation du fichier memorisant l'alarme concernee */
         vl_FD_Ala = open (pl_NomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP);
         close ( vl_FD_Ala );
      }
      else
      {
         /* Test s'il s'agit d'une disparition d'alarme */
         if ( ( va_Etat == XDC_FAUX ) && ( vl_NouvelleAlarme != XDC_OK ) )
         {
            /* Suppression du fichier memorisant l'alarme concernee */
            XZSS_06DetruireFichier ( pl_NomFichier);
            /* Appel de la fonction remontee des alames a OPEN VIEW */
            ex_remonte_al_ovw ( XDC_FAUX, pa_NomMachine );
         }
         else
         {
            if ( (vm_InitAlarme == XDC_NOK) && (ex_existe_alarme ( &vl_Alarme ) == XDC_NOK) )
            {
               XZST_03EcritureTrace(XZSTC_DEBUG1,"Renvoi Alarme %lf TypeEqt:%d NumEqt:%d Type:%d Etat:%d ",
               					   vl_horodate,
						   va_TypeEqt,
						   va_NumEqt,
						   va_Type,
						   va_Etat);
               ex_ajout_liste_alarme ( &vl_Alarme );
               ex_wfic_liste_alarme ( pa_NomMachine ); 
            }
            return ( XDC_NOK );
         }
      }
      
      /*A Evenement sur une alarme rupture de liaison */
      if ( va_Type == XZAMC_NON_REP )
      {
         /* Envoi du message RTWorks sur XDM_ETAT_LIAISON */
         XZST_03EcritureTrace(XZSTC_INFO,"%s : %s : Envoi message XDM_ETAT_LIAISON effectue avec succes", version, fonction);
   		
   	 sprintf ( pl_DG_Liaison, "%s_%s", XDG_ETAT_LIAISON, pa_NomMachine );
   	 /* Envoi du message XDM_ETAT_LIAISON en fournissant les valeur recues */
   	 if (!TipcSrvMsgWrite ( pl_DG_Liaison,
                             XDM_IdentMsg(XDM_ETAT_LIAISON),
                             XDM_FLG_SRVMSGWRITE,
                             T_IPC_FT_INT2, va_NumEqt,
                             T_IPC_FT_INT2, va_TypeEqt,
                             T_IPC_FT_INT2, va_Etat,
                             NULL))
         {
             XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_LIAISON non effectue" );
         }
         else
         {
             XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_LIAISON effectue" );
         }
   	 TipcSrvFlush();

   	 XZST_03EcritureTrace(XZSTC_DEBUG1,"%s : %s : Station = %d\tType = %d\tEtat Liaison = %d\n", 
   					version, 
   					fonction, 
   					va_NumEqt,
   					va_TypeEqt,
   					va_Etat );
     }
     
      ex_ajout_liste_alarme ( &vl_Alarme );
      ex_wfic_liste_alarme ( pa_NomMachine ); 
      
      /* Si le fichier d'enregistrement des alarmes existe */
      sprintf ( pl_NomFichier, "%s/%s_%s", XZEXC_PATH_FIC_ALARME, XDF_Alarme, pa_NomMachine );
      if ( !access ( pl_NomFichier, F_OK ) )
      {
         
         /* Ouverture du fichier en Lecture et ecriture */
         while ( ((vl_Fd = open ( pl_NomFichier, O_RDWR ) ) <= 0) && (vl_CptEssai++ < EALAC_NB_ESSAI_OPEN) )
         {
            /* Attente avant nouvel essai d'ouverture fichier */
            XZSS_22Attendre ( 500 );
         }
         /* Interdire l'acces au fichier en ecriture */
         chmod ( pl_NomFichier, S_IRUSR | S_IRGRP );
         /* Lecture de l'entete du fichier afin de connaitre la position du prochain enregistrement */
         if ( ex_LireLigneFich ( vl_Fd, pl_Entete, EALAC_LON_TEXTE_ALARME+2 ) == XZEXC_EOF ) 
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "Lecture entete fichier d'enregistrement des alarmes incorrect" );
            close ( vl_Fd );         
            chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
            return ( XDC_NOK );
         }
         else
         {
            /* Extraction de la valeur du pointeur */
            if ( sscanf ( pl_Entete, vl_Entete, &vl_PtrFile ) != 1 )
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Pointeur fichier d'enregistrement des alarmes incorrect" );
                close ( vl_Fd );         
                chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
                return ( XDC_NOK );
            }
            else
            {
               /* Positionnement du pointeur du fichier */
                if ( !lseek ( vl_Fd, vl_PtrFile , SEEK_SET ) )
               {
                   XZST_03EcritureTrace( XZSTC_WARNING, "Erreur positionnement sur fichier d'enregistrement des alarmes incorrect" );
                   close ( vl_Fd );         
                   chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
                   return ( XDC_NOK );
               }
            }
         }
      }
      else
      {
          /* Creation du fichier d'enregistrement des alarmes */
         if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR | O_CREAT ) ) < 0 )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer le fichier d'enregistrement des alarmes" );
            return ( XDC_NOK );
         }
         else
         {
            /* Ecriture de l'entete dans le fichier */
            sprintf ( pl_Entete, vl_Entete, vl_PtrFile );
            if ( write ( vl_Fd, pl_Entete, strlen(pl_Entete) ) < 0 )
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Ecriture entete fichier d'enregistrement des alarmes incorrect" );
               close ( vl_Fd );         
               chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
               return ( XDC_NOK );
            }
            else
            {
               /* Mise a jour du pointeur */
               vl_PtrFile = strlen(pl_Entete);
            }
         }
      }


       /* Enregistrement de l'alarme */
      /* Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
      XZSM_11ConversionHorodate ( vl_horodate, &pl_StrHorodate );
      XZEX03_nom_eqt ( va_TypeEqt, vl_NomTypeEqt );
      ex_nom_equipement ( va_TypeEqt, va_NumEqt, pa_NomMachine, vl_NomEqt, vl_NomMachine );
      ex_lecture_fichier_alarme ( va_Type, &vl_InfoAlarme );
      sprintf ( pl_LigneFich, "%s, %s, %d, %s, %s %s %s %s",
                	pl_StrHorodate,
			vl_NomTypeEqt,
			va_NumEqt,
			vl_NomEqt,
			(va_Etat == XDC_DEBUT_ALARME) ? "DEBUT ALARME" : "FIN   ALARME",
			(vl_InfoAlarme.Urgence == XZAMC_MINEURE) ? "MINEURE" : "MAJEURE" ,
			vl_InfoAlarme.Libelle,
			(va_ArgAlarme != NULL) ?  va_ArgAlarme : "" );         
      XZST_03EcritureTrace( XZSTC_DEBUG3, "Alarme: %s", pl_LigneFich );
      
 
      vl_LgTexte = strlen(pl_LigneFich);
      memset ( &pl_LigneFich[vl_LgTexte], ' ', EALAC_LON_TEXTE_ALARME-vl_LgTexte );
      memcpy ( &pl_LigneFich[EALAC_LON_TEXTE_ALARME-3], "\n\0", 2 );
      /* Si taille maxi du fichier d'alarme atteinte */
      if ( vl_PtrFile + strlen(pl_LigneFich) > EALAC_TAILLE_FIC_ALARME )
      {
         /* Postionnement du pointeur au debut */
         vl_PtrFile = strlen(pl_Entete)+1;
         if ( !lseek ( vl_Fd, vl_PtrFile , SEEK_SET ) )
         {
             XZST_03EcritureTrace( XZSTC_WARNING, "Erreur positionnement sur fichier d'enregistrement des alarmes incorrect" );
             close ( vl_Fd );         
             chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
             return ( XDC_NOK );
         }
      }
      
      if ( write ( vl_Fd, pl_LigneFich, strlen(pl_LigneFich) ) < 0 )
      {
          XZST_03EcritureTrace( XZSTC_WARNING, "Ecriture dans fichier d'enregistrement des alarmes incorrect" );
          close ( vl_Fd );         
          chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
          return ( XDC_NOK );
      }
      else
      {
           /* Mise a jour du pointeur */
          vl_PtrFile += strlen(pl_LigneFich);
          /* Enregistrement dans le fichier du nouveau pointeur */
          sprintf ( pl_LigneFich, vl_Entete, vl_PtrFile );
          /* Positionnement du pointeur du fichier */
          if ( lseek ( vl_Fd, 0, SEEK_SET ) )
          {
              XZST_03EcritureTrace( XZSTC_WARNING, "Erreur positionnement sur fichier d'enregistrement des alarmes incorrect" );
              close ( vl_Fd );         
              chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
              return ( XDC_NOK );
          }
          else
          {
              if ( write ( vl_Fd, pl_LigneFich, strlen(pl_LigneFich) ) < 0 )
             {
                XZST_03EcritureTrace( XZSTC_WARNING, "Ecriture entete fichier d'enregistrement des alarmes incorrect" );
                close ( vl_Fd );         
                chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
                return ( XDC_NOK );
             }
             else
             {   
                  /* Fermeture du fichier */
                 close ( vl_Fd );         
                 /* Remettre l'acces au fichier en ecriture */
                 chmod ( pl_NomFichier, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP );
             }
          }
       }
      
      return ( XDC_OK );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_traite_alarme_com ( 	XDY_NomMachine 	va_NomMachine,
				int		va_typeeqt,
				int		va_noeqt,
      				int		va_typ_rep,
      				int		va_nbnonrep,
      				int		va_nberrcrc,
      				int		va_typetrame,
      				int		va_Mode_Fonct,
				int		va_SiteGestion)
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
char   			vl_NbErr[10] = "";
XDY_Horodate		vl_Horodate = 0.0;
int			vl_JS = 0;

   /* Si trame LCR retour a l'appelant */
   if ( va_typetrame != XZEXC_TRAME_FEN_LCR )
   {
      /* Lecture vl_Horodate */
      XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_JS , &vl_Horodate );
      if ( va_typ_rep == XZEXC_TRAME_NON_TRANSM )
      {
         XZST_03EcritureTrace( XZSTC_INFO, " Erreur trame non transmise ou non comprise sur l'eqt no %d",va_noeqt);
         XZST_03EcritureTrace( XZSTC_INFO, " Emission alarme vers le site %d",va_SiteGestion);
         /* Envoi debut d'alarme trame non transmise */
         ex_env_alarme  ( 	va_NomMachine,
				vl_Horodate,
				va_typeeqt,
				va_noeqt,
				XZAMC_NON_REP,
				XDC_VRAI,
				NULL,
				va_SiteGestion);
	 return ( XDC_NOK );
     }
     else
     {
         /* Envoi fin d'alarme trame non transmise */
         ex_env_alarme  ( 	va_NomMachine,
				vl_Horodate,
				va_typeeqt,
				va_noeqt,
				XZAMC_NON_REP,
				XDC_FAUX,
				NULL,
				va_SiteGestion);
           
        /*A Envoi alarme fugitive non reponse si erreur */
        if ( (va_nbnonrep > 1) && (va_nberrcrc == 0) )
        {
           /*A Envoi debut de l'alarme non reponse */
           sprintf ( vl_NbErr, "%d",  va_nbnonrep );
           ex_env_alarme  ( 	va_NomMachine,
				vl_Horodate,
				va_typeeqt,
				va_noeqt,
				XZAMC_ERR_TRANS,
				XDC_VRAI,
				vl_NbErr,
				va_SiteGestion);
           ex_env_alarme  ( 	va_NomMachine,
				vl_Horodate,
				va_typeeqt,
				va_noeqt,
				XZAMC_ERR_TRANS,
				XDC_FAUX,
				vl_NbErr ,
				va_SiteGestion);
	   return ( XDC_NOK );
	}
	
        /*A Envoi alarme fugitive erreur crc si erreur CRC */
	if ( va_nberrcrc > 1 )
	{
           sprintf ( vl_NbErr, "%d", va_nberrcrc );
           ex_env_alarme  ( 	va_NomMachine,
				vl_Horodate,
				va_typeeqt,
				va_noeqt,
				XZAMC_ERR_CRC,
				XDC_VRAI,
				vl_NbErr,
				va_SiteGestion);
           ex_env_alarme  ( 	va_NomMachine,
				vl_Horodate,
				va_typeeqt,
				va_noeqt,
				XZAMC_ERR_CRC,
				XDC_FAUX,
				vl_NbErr,
				va_SiteGestion);
	   return ( XDC_NOK );
         }
      }
   }
   
   return ( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_env_etat_liaison ( 	XDY_NomMachine 	pa_NomMachine,
				XDY_Datagroup	pa_NomDG,
				XDY_Eqt		va_NumEqt,
				XDY_Eqt		va_TypeEqt )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
char 			pl_NomFichier[XDC_PATH_ABS_SIZE] = "";
XDY_NomEqt		vl_NomTypeEqt = "";
XDY_Booleen		vl_Etat = XDC_FAUX;


      ex_nom_eqt ( va_TypeEqt, vl_NomTypeEqt );
      if ( va_TypeEqt == XDC_EQT_PCT )
         strcpy ( vl_NomTypeEqt, "picto" );
      /* Construction du nom du fichier memorisant l'etat de l'alarme */
      sprintf ( pl_NomFichier, "%s/Ala_%s_%s_%03d_%03d",
      				XZEXC_PATH_FIC_ALARME,  
      				pa_NomMachine,
      				vl_NomTypeEqt,
      				va_NumEqt,
      				XZAMC_NON_REP );
      				
      /*A Test si nouvelle alarme */
      if ( !access ( pl_NomFichier, F_OK ) )
      {
         vl_Etat = XDC_VRAI;
      }

      /* Envoi du message XDM_ETAT_LIAISON en fournissant les valeur recues */
 
      if (!TipcSrvMsgWrite ( pa_NomDG,
                             XDM_IdentMsg(XDM_ETAT_LIAISON),
                             XDM_FLG_SRVMSGWRITE,
                             T_IPC_FT_INT2, va_NumEqt,
                             T_IPC_FT_INT2, va_TypeEqt,
                             T_IPC_FT_INT2, vl_Etat,
                             NULL))
      {
          XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_LIAISON non effectue" );
          return (XDC_NOK);
      }
      else
      {
          XZST_03EcritureTrace(XZSTC_INFO,"Envoi message XDM_ETAT_LIAISON effectue" );
      }

      return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_existe_alarme ( EX_MALAT_ALARME * 	pa_Alarme )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
EX_MALAT_ALARME *       pl_Alarme = NULL;
EX_MALAT_ALARME *       pl_Enr    = NULL;
EX_MALAT_ALARME *       pl_ExEnr  = NULL;

   /*A Recherche si l'alarme existe dans la liste */
   for ( pl_Alarme = pm_ListeAlarmeInit; pl_Alarme != NULL; pl_Alarme = pl_Alarme->Suivant )
   {
      if ( (pl_Alarme->TypeEqt == pa_Alarme->TypeEqt) &&
           (pl_Alarme->NumEqt == pa_Alarme->NumEqt) &&
           (pl_Alarme->TypeAlarme == pa_Alarme->TypeAlarme) )
      {
          return ( XDC_OK );
      }
   }

   /*A si l'alocaltion memoire est ok */
   if ( ( pl_Alarme = (EX_MALAT_ALARME *)malloc ( sizeof(EX_MALAT_ALARME) ) ) != NULL)
   {
      /*A copie enegistrement */
      memcpy ( pl_Alarme, pa_Alarme, sizeof(*pl_Alarme) );
      
      /*A Insertion en queue de liste */
      for ( pl_Enr = pl_ExEnr = pm_ListeAlarmeInit; pl_Enr != NULL; pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant );

      /*A si la place d'insertion est la tete de liste */
      if ( pl_Enr == pm_ListeAlarmeInit )
      {
         pl_Alarme->Suivant = pm_ListeAlarmeInit;
         pm_ListeAlarmeInit = pl_Alarme;
      }
      /*A sinon insertion nominal */
      else
      {
         pl_Alarme->Suivant = pl_ExEnr->Suivant;
         pl_ExEnr->Suivant  = pl_Alarme;
      }
   }
   else
   {
      XZST_03EcritureTrace (XZSTC_WARNING,"ex_existe_alarme : Probleme allocation memoire" );
   }
   
   return ( XDC_NOK );


}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Fonction ajoutee fournissant aux taches externes un acces
*   a la fonction generale de reemission des alarmes du module.
*
--------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_traiter_alarmes ( )
{
  ex_reemission_alarme ( 0 );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_reemission_alarme ( int va_numSignal )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   if ( pm_ListeAlarme != NULL )
   {
      /* Envoi de l'alarme par XZAM01_Ajout_Alarme */
/*      XZST_03EcritureTrace( XZSTC_INTERFACE, "Appel XZAM01_Ajout_Alarme avec %lf TypeEqt:%d NumEqt:%d Type:%d Etat:%d Machine:%s SiteGestion:%d",
         		    pm_ListeAlarme->Horodate,
		            pm_ListeAlarme->TypeEqt,
			    pm_ListeAlarme->NumEqt,
			    pm_ListeAlarme->TypeAlarme,
			    pm_ListeAlarme->Etat,
			    pm_ListeAlarme->NomMachine,
			    pm_ListeAlarme->SiteGestion);
*/			    
      XZAM01_Ajout_Alarme ( pm_ListeAlarme->Horodate,
		            pm_ListeAlarme->TypeEqt,
			    pm_ListeAlarme->NumEqt,
			    pm_ListeAlarme->TypeAlarme,
			    pm_ListeAlarme->Etat,
			    pm_ListeAlarme->NomMachine,
			    pm_ListeAlarme->SiteGestion);

      /* Armement du time_out */
      XZSS_14ArmerTimeout ( EALAC_TIMEOUT_ALARME );
   }
   else
      XZSS_18AnnulerTimeout ();
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_abt_acknoledge_alarme ( XDY_NomMachine	va_NomMachine, XDY_TypeEqt    		va_TypeEqt )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
static char *fonction = "ex_abt_acknoledge_alarme" ;

XDY_Datagroup   pl_NomDG   = "";
T_BOOL          vl_EtatDg  = FALSE;
XDY_Texte       pl_TypeEqt = "";
 
   /*A Abonnement au Datagroup  */
   /*A Construction du DataGroup XDG_ACKNOLEDGE_ALA_<NomMachine>_<TypeEqt> */
   /*A Construction du nom du type de l'equipement */
   if ( XZEX03_nom_eqt ( va_TypeEqt, pl_TypeEqt ) != XDC_OK )
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "%s : %s : XZEX03_nom_eqt avec type:%d retourne NOK", version, fonction, va_TypeEqt);
   }
   sprintf ( pl_NomDG, "%s_%s_%s", XDG_ACKNOLEDGE_ALA, va_NomMachine, pl_TypeEqt ) ;
   /*A Joindre ce datagroup s'il n'est pas deja joint */
   TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   if ( !vl_EtatDg )
   {
      /*A Joindre ce datagroup  */
      if ( !XDG_JoindreDG( pl_NomDG ) )
         XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
      else
         XZST_03EcritureTrace ( XZSTC_INFO, "Abonnement au Datagroup %s ", pl_NomDG );
   }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_init_alarme ( XDY_NomMachine	va_NomMachine, char * pa_NomTache )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
 XDY_NomMachine	pl_NomServeur    = "";
 FILE        	*pl_Cmd          = NULL; /* Descripteur du canal logique  */
 char        	pl_Commande[128] = "";   /* Commande a executer */
 char        	pl_ResCmd[128]   = "";   /* Variable temporaire */
 
   strcpy ( vm_NomTache, pa_NomTache );

   /*A Armement du CallBack sur  XDM_Acknoledge_Ala */
   if ( (TipcSrvProcessCbCreate (XDM_IdentMsg(XDM_Acknoledge_Ala), (void (*)())ex_acknoledge_alarme,(T_CB_ARG)NULL) ) == NULL )
      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_Acknoledge_Ala ." );
   else
      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_Acknoledge_Ala ." );
   
   if ( strstr (pa_NomTache, XZEXC_ADR_SOCK_TEINI ) != NULL )
   {
      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Pas d'Init ." );
      vm_InitAlarme = XDC_OK;
      /*A Lecture du fichier Acknoledge Alarme */
      ex_rfic_liste_alarme ( va_NomMachine );
      return;
   }
   
   if ( (ex_lire_serveur(&pl_NomServeur) == XDC_OK) && (ex_presence_machine(pl_NomServeur) == XDC_OK) )
   {
      if ( strstr (pa_NomTache, ".xs" ) != NULL )
         sprintf ( pl_Commande, "remsh %s -n \"touch %s/%s/%s_%s\"", pl_NomServeur, XZEXC_PATH_DIFF_DYN, pl_NomServeur, va_NomMachine, vm_NomTache );
      else
         sprintf ( pl_Commande, "remsh %s -n \"ls %s/%s/%s_%ss\"", pl_NomServeur, XZEXC_PATH_DIFF_DYN, pl_NomServeur, va_NomMachine, vm_NomTache );
      if ((pl_Cmd = popen(pl_Commande,"r")) != NULL)
      {
         /* recupere le resultat de cette commande */
         fgets(pl_ResCmd,sizeof(pl_ResCmd)-1,pl_Cmd);
         pclose(pl_Cmd);
         if ( strstr(pa_NomTache, ".xs") == NULL )
         {
            if ( strstr(pl_ResCmd,vm_NomTache) != NULL )
            {
               sprintf ( pl_Commande, "remsh %s -n \"rm %s/%s/%s_%ss\"", pl_NomServeur, XZEXC_PATH_DIFF_DYN,  pl_NomServeur, va_NomMachine, vm_NomTache );
               if ((pl_Cmd = popen(pl_Commande,"r")) != NULL) pclose(pl_Cmd);
            }
            else
            {
               XZST_03EcritureTrace ( XZSTC_DEBUG1, "Pas d'Init ." );
               vm_InitAlarme = XDC_OK;
               /*A Lecture du fichier Acknoledge Alarme */
               ex_rfic_liste_alarme ( va_NomMachine );
            }
         }
      }
   }
   /*A Emission des alarmes non acquittees 
   ex_reemission_alarme (SIGALRM);*/
   
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_ajout_liste_alarme ( EX_MALAT_ALARME * 	pa_Alarme )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
EX_MALAT_ALARME *       pl_Alarme = NULL;
EX_MALAT_ALARME *       pl_Enr    = NULL;
EX_MALAT_ALARME *       pl_ExEnr  = NULL;

    ex_abt_acknoledge_alarme ( pa_Alarme->NomMachine, pa_Alarme->TypeEqt );

   /*A si l'alocaltion memoire est ok */
   if ( ( pl_Alarme = (EX_MALAT_ALARME *)malloc ( sizeof(EX_MALAT_ALARME) ) ) != NULL)
   {
      /*A copie enegistrement */
      memcpy ( pl_Alarme, pa_Alarme, sizeof(*pl_Alarme) );
      
      /*A Insertion en queue de liste */
      for ( pl_Enr = pl_ExEnr = pm_ListeAlarme; pl_Enr != NULL; pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant );

      /*A si la place d'insertion est la tete de liste */
      if ( pl_Enr == pm_ListeAlarme )
      {
         pl_Alarme->Suivant = pm_ListeAlarme;
         pm_ListeAlarme     = pl_Alarme;
      }
      /*A sinon insertion nominal */
      else
      {
         pl_Alarme->Suivant = pl_ExEnr->Suivant;
         pl_ExEnr->Suivant  = pl_Alarme;
      }
   }
   else
   {
      XZST_03EcritureTrace (XZSTC_WARNING,"ex_ajout_liste_alarme : Probleme allocation memoire" );
      return;
   }
   
   /*A Emission des alarmes non acquittees */
   if ( pm_ListeAlarme->Suivant == NULL )
      ex_reemission_alarme (SIGALRM);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_acknoledge_alarme ( T_IPC_CONN                      va_Cnx,
                           T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
                           T_CB_ARG                        pa_ARG )
 
/*
* ARGUMENTS EN ENTREE :
*  
*       Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
/* Variables locales pour messages RTWks */

XDY_Horodate    	vl_Horodate;    
#ifdef _TIBCO_EMS
XDY_Mot    		vl_TypeEqt;    
XDY_Mot   	      	vl_NumEqt;          
XDY_Mot  		vl_TypeAlarme;      
XDY_Mot     		vl_Etat;   
#else
T_INT2    		vl_TypeEqt;    
T_INT2   	      	vl_NumEqt;          
T_INT2  		vl_TypeAlarme;      
T_INT2     		vl_Etat;   
#endif

XDY_NomMachine		vl_NomMachine = "";        
EX_MALAT_ALARME *       pl_Alarme    = NULL;
EX_MALAT_ALARME *       pl_ExAlarme  = NULL;

   /*A
   ** Recuperation du contenu du message XDM_Acknoledge_Ala
   */
   if (TipcMsgRead(pa_Data->msg,   T_IPC_FT_REAL8, &vl_Horodate,
                                   T_IPC_FT_INT2, &vl_TypeEqt,
                                   T_IPC_FT_INT2, &vl_NumEqt,
                                   T_IPC_FT_INT2, &vl_TypeAlarme,
                                   T_IPC_FT_INT2, &vl_Etat,
                                   NULL))
   {
       XZST_03EcritureTrace (XZSTC_DEBUG1,"Reception Acknoledge Alarme  Horodate = %lf TypeEqt = %d Eqt = %d TypeAlarme = %d Etat = %d",
                                                                        vl_Horodate,
                                                                        vl_TypeEqt,
                                                                        vl_NumEqt,
                                                                        vl_TypeAlarme,
                                                                        vl_Etat);
   }
   else
   {
       XZST_03EcritureTrace (XZSTC_WARNING," Echec Reception Acknoledge Alarme " );
       return;
   }
   
   /*A Suppression de l'alarme dans la liste */
   for ( pl_Alarme = pl_ExAlarme = pm_ListeAlarme; pl_Alarme != NULL; pl_ExAlarme = pl_Alarme, pl_Alarme = pl_Alarme->Suivant )
   {
      if ( (pl_Alarme->Horodate == vl_Horodate) &&
           (pl_Alarme->TypeEqt == vl_TypeEqt) &&
           (pl_Alarme->NumEqt == vl_NumEqt) &&
           (pl_Alarme->TypeAlarme == vl_TypeAlarme) &&
           (pl_Alarme->Etat == vl_Etat) )
      {
         strcpy ( vl_NomMachine, pl_Alarme->NomMachine );
         /*B si suppression en tete de liste */
         if ( (pl_ExAlarme == pm_ListeAlarme) && (pl_ExAlarme == pl_Alarme) )
         {
            pm_ListeAlarme     = pl_Alarme->Suivant;
         }
         /*B sinon suppression nominal */
         else
         {
            pl_ExAlarme->Suivant = pl_Alarme->Suivant;
         }
         free ( pl_Alarme );
         ex_wfic_liste_alarme ( vl_NomMachine );
         /*A Emission des alarmes non acquittees */
         ex_reemission_alarme (SIGALRM);
         break;
      }
   }
   
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void ex_wfic_liste_alarme ( XDY_NomMachine	va_NomMachine )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
* creer le fichier Acknoledge des alarmes
*
------------------------------------------------------*/
{
EX_MALAT_ALARME *       pl_Alarme    = NULL;
int                     vl_Fd 	     = 0;
char                    pl_NomFichier[XDC_PATH_ABS_SIZE];
XDY_Texte		pl_LigneFich = "";


   /* Si le fichier d'enregistrement des alarmes existe */
   sprintf ( pl_NomFichier, "%s/Ack_%s_%s", XZEXC_PATH_FIC_ALARME, va_NomMachine, vm_NomTache );
   if ( !access ( pl_NomFichier, F_OK ) )
   {
      XZSS_06DetruireFichier ( pl_NomFichier );
   }
   
   if ( pm_ListeAlarme == NULL )
      return;
      
   /* Creation du fichier d'enregistrement des alarmes */
   if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) ) < 0 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer le fichier Acknoledge des alarmes" );
      return;
   }

   /*A Ecriture de toutes les alarmes de la liste */
   for ( pl_Alarme = pm_ListeAlarme; pl_Alarme != NULL; pl_Alarme = pl_Alarme->Suivant )
   {
      sprintf ( pl_LigneFich, "%lf %hd %hd %hd %hd %s %hd\n",	
      					pl_Alarme->Horodate,
		            		pl_Alarme->TypeEqt,
			    		pl_Alarme->NumEqt,
			    		pl_Alarme->TypeAlarme,
			    		pl_Alarme->Etat,
			    		pl_Alarme->NomMachine,
					pl_Alarme->SiteGestion);
      XZST_03EcritureTrace( XZSTC_DEBUG1, "Acknoledge: %s", pl_LigneFich );
      
      if ( write ( vl_Fd, pl_LigneFich, strlen(pl_LigneFich) ) < 0 )
      {
          XZST_03EcritureTrace( XZSTC_WARNING, "Ecriture dans fichier Acknoledge des alarmes incorrect" );
          close ( vl_Fd );         
          return;
      }
      
   }
   
   close ( vl_Fd );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void ex_rfic_liste_alarme ( XDY_NomMachine	va_NomMachine )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
* lire le fichier Acknoledge des alarmes
*
------------------------------------------------------*/
{
EX_MALAT_ALARME         vl_Alarme;
int                     vl_Fd 	      = 0;
char                    pl_NomFichier[XDC_PATH_ABS_SIZE];
XDY_Texte		pl_LigneFich  = "";
XDY_Mot			vl_Etat       = 0;
XDY_Mot			vl_TypeEqt    = 0;
XDY_Mot			vl_TypeAlarme = 0;
XDY_Mot			vl_SiteGestion;

   /* Si le fichier d'enregistrement des alarmes existe */
   sprintf ( pl_NomFichier, "%s/Ack_%s_%s", XZEXC_PATH_FIC_ALARME, va_NomMachine, vm_NomTache );
   if ( access ( pl_NomFichier, F_OK ) )
   {
      return;
   }
   /* Creation du fichier d'enregistrement des alarmes */
   if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR ) ) < 0 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier Acknoledge des alarmes" );
      return;
   }

   /*A Lecture de chaque ligne du fichier */
   while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF )
   {
      /*A Lire ligne suivante si c'est une ligne de commentaire */
      if ( *pl_LigneFich != XDF_COMMENTAIRE )
      {
         /*A Extraire les parametre de l'alarme */
         sscanf ( pl_LigneFich, "%lf %hd %hd %hd %hd %s %hd\n",	
      					&vl_Alarme.Horodate,
		            		&vl_TypeEqt,
			    		&vl_Alarme.NumEqt,
			    		&vl_TypeAlarme,
			    		&vl_Etat,
			    		vl_Alarme.NomMachine,
					&vl_SiteGestion);
	 vl_Alarme.Etat       = vl_Etat;
	 vl_Alarme.TypeEqt    = vl_TypeEqt;
	 vl_Alarme.TypeAlarme = vl_TypeAlarme;
	 vl_Alarme.SiteGestion = vl_SiteGestion;
	 ex_ajout_liste_alarme ( &vl_Alarme );
         XZST_03EcritureTrace( XZSTC_DEBUG1, "Relecture de: %lf %d %d %d %d %s %d",	
      					vl_Alarme.Horodate,
		            		(int) vl_Alarme.TypeEqt,
			    		(int) vl_Alarme.NumEqt,
			    		(int) vl_Alarme.TypeAlarme,
			    		(int) vl_Alarme.Etat,
			    		vl_Alarme.NomMachine,
					(int) vl_Alarme.SiteGestion);
      }
   }
   
   close ( vl_Fd );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_sup_alarme_eqt_HS ( 
		 	XDY_NomMachine	pa_NomMachine,
			XDY_TypeEqt	va_TypeEqt,
			XDY_Eqt		va_NumEqt,
			XDY_District	va_SiteGestion)


/*
* ARGUMENTS EN ENTREE :
*
*  XDY_NomMachine	pa_NomMachine	: Nom machine
*  XDY_TypeEqt		va_TypeEqt	: Type d'equipement 
*  XDY_Eqt		va_NumEqt	: Numero d'equipement 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Appel sur changement etat de service equipement 
*   
*
* FONCTION 
* Verifie dans le repertoire des alarmes si des alarmes de l'equipement concerne sont presente
* Si oui appel de ex_env_alarme avec etat alarme = XDC_FAUX
*
------------------------------------------------------*/
{
XDY_Nom	    	vl_NomEqt = ""; 
FILE        	*pl_pipe;          /* Descripteur du canal logique entre le 
                                   processus et la commande a executer */
char        	vl_commande[128];  /* Commande a executer */
char        	vl_tampon[128];    /* Variable temporaire */
int        	 vl_Fd 	      = 0;
char        	pl_NomFichier[XDC_PATH_ABS_SIZE];
XDY_TypeAlarme	vl_TypeAlarme = 0;


 XZST_03EcritureTrace( XZSTC_FONCTION, "IN  : ex_sup_alarme_eqt_HS avec NomMachine:%s Type:%d Eqt:%d", 
 					pa_NomMachine, va_TypeEqt, va_NumEqt );
 
 /*A Recherche des alarmes dans le repertoire ala */
 ex_nom_eqt ( va_TypeEqt, vl_NomEqt );
 if ( va_TypeEqt == XDC_EQT_PCT )
    strcpy ( vl_NomEqt, "picto" );
 
 
 sprintf ( pl_NomFichier, "la_%s_%s_%03d_", pa_NomMachine, vl_NomEqt, va_NumEqt );
 sprintf ( vl_commande, "ls %s | grep %s*", XZEXC_PATH_FIC_ALARME, pl_NomFichier );

 XZST_03EcritureTrace( XZSTC_DEBUG2, "ex_sup_alarme_eqt_HS cmd:<%s>", vl_commande );

 if ( (pl_pipe = popen(vl_commande,"r")) != NULL )
 {
    /* Lecture du premier elements du pipe pour eliminer le compte rendu de la commande */
    while ( fgets(vl_tampon,128,pl_pipe) != NULL )
    {
       XZST_03EcritureTrace( XZSTC_DEBUG2, "ex_sup_alarme_eqt_HS : reponse cmd : <%s> ", vl_tampon );
       if ( (vl_TypeAlarme = atoi (&vl_tampon[strlen(pl_NomFichier)+1])) != 0 )
       {
          XZST_03EcritureTrace( XZSTC_DEBUG2, "ex_sup_alarme_eqt_HS : suppression de <%s%03d> ", pl_NomFichier, vl_TypeAlarme );
          ex_env_alarme ( 	pa_NomMachine,
				0,
				va_TypeEqt,
				va_NumEqt,
				vl_TypeAlarme,
				XDC_FAUX,
				NULL,
				va_SiteGestion);
       }
    }
    pclose ( pl_pipe );
 }
 
 XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : ex_sup_alarme_eqt_HS" );
   
}
