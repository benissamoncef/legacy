/*E*/
/*Fichier :  $Id: eini_cfg.c,v 1.44 2021/06/24 09:32:54 pc2dpdy Exp $      Release : $Revision: 1.44 $        Date : $Date: 2021/06/24 09:32:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de creation des fichiers de config des equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	26 Sep 1994	: Creation
* Mismer.D	version 1.2	14 Dec 1994	:
* Mismer.D	version 1.3	23 Jan 1995	:
* Mismer.D	version 1.4	25 Jan 1995	:
* Mismer.D	version 1.5	25 Jan 1995	: RAS
* Mismer.D	version 1.6	25 Jan 1995	: RAS
* Mismer.D	version 1.7	25 Jan 1995	: RAS
* Mismer.D	version 1.8	25 Jan 1995	: RAS
* Mismer.D	version 1.9	26 Jan 1995	:
* Mismer.D	version 1.10	28 Jan 1995	:
* Mismer.D	version 1.11	09 Fev 1995	:
* Mismer.D	version 1.12	14 Fev 1995	:
* Mismer.D	version 1.13	09 Mar 1995	:
* Mismer.D	version 1.14	09 Mar 1995	: RAS
* Mismer.D	version 1.15	13 Avr 1995	:
* Fontaine.C	version 1.16	19 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.17	19 Mai 1995	: Ajout Liste utilisateurs pour LT
* Volcic.F	version 1.18	22 Mai 1995	: Modif Liste utilisateurs pour LT
* Mismer.D	version 1.19	23 Jun 1995	: Modif fichier machine suppleante
* Mismer.D      version 1.20    06 Sep 1996     : Modif pour Extension Ouest   (RADT_W)
* Mismer.D      version 1.21    10 Sep 1996     : Modif pour Extension Ouest Suite  (RADT_W)
* Mismer.D	version 1.23	07 Nov 1996	: Modif METEO (DEM/1232)
* Mismer.D	version 1.24	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.25	15 Jan 1998	: Modif PAL (DEM/1539)
* Mismer.D	version 1.27	19 Oct 1998	: Ajout GTC pour PCS (DEM/1700)
						  Suppression de Satir 
* JMG		version 1.29	11/10/05	: ajout PMVA BAF BAD
* AAZ						Nov 2007	: DEM 664
* JPL	19/11/10 : ec_config_all : liberation memoire des machines supprimees (lie a DEM 961)  1.31
* PNI	25/01/11 : Suppression du traitement pour la telephonie DEM965 1.32
* VR	20/12/11		: Ajout PRV (DEM/1016)
* JPL	15/02/12 : Correction gestion des lignes serie (port de numero max) (DEM 1018)  1.34
* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)
* PNI	22/02/16 : patch pour la régionalisation et conserver pour radt CA à 9600 (DEM1150) 1.37
* JPL	24/03/17 : Suppression de la generation de la configuration video (DEM 1189)  1.38
* JMG	24/02/18 : ajout CFE CTRL_FEUX 1.39 DEM1284
* JMG	07/11/18 : gestion bonne vitesse sur les PMV 1.40
* RGR   12/02/19 : Remise de la generation de la configuration video
* JMG	06/01/20 : correctif SAE-190 + IMU DEM-SAE155 1.41
* LCL	18/06/21 : Fixs teini / Machine_Sup / Config_Lignes / Presence_Equip  SAE-295 1.42
* LCL	24/06/21 : Fix 
* LCL	07/07/21 : Fix probleme lignes IP dans les fichiers Config_Lignes_XXX DEM-SAE306 1.44
* GGY	13/10/23 : Ajout test site pour envoi XDM_EINIT (DEM483)
* LCL	19/10/23 : Ajout test site pour envoi XDM_EINIT (DEM483)
* GGY	12/12/23 : Fix nom serveur POLT, ST, LT... (DEM-559)
------------------------------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"
#include "eini_syn.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_cfg.c,v 1.44 2021/06/24 09:32:54 pc2dpdy Exp $ : eini_cfg" ;
EINI_PARAM_LIGNE	vg_ParamEqt[XZECC_MAX_EQT];
int			vg_InitTache = XDC_NOK;

/* declaration de variables globales */
extern XDY_Mot		vg_SitePrincipal;

/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_lire_parametre_LS ( 	 )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture du fichier XDF_ParamLS
*
------------------------------------------------------*/
{
   static char *version = "$Id: eini_cfg.c,v 1.44 2021/06/24 09:32:54 pc2dpdy Exp $ : lire_machine_sup" ;
   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Commentaire		pl_LigneFich;
   int				vl_NbConfLigne = 0;
   XDY_NomMachine       	pl_NomMachine;
   int				vl_NbEqt = 0;
   

       /*A Recuperation du nom de la machine */
       if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
       {
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
       }
	 
      /*A
       * Lecture du fichier de configuration  XDF_ParamLS
       */
      /*A Construction du nom du fichier XDF_ParamLS  */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_DIFFUSION, XDF_ParamLS );
      
      /*A Ouverture du fichier XDF_ParamLS  */
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
             if ( sscanf ( pl_LigneFich, "%s %s %d %d %d", 
             				vg_ParamEqt[vl_NbEqt].Equipement,
                                        vg_ParamEqt[vl_NbEqt].Vitesse,
                                        &vg_ParamEqt[vl_NbEqt].NbEssais,
                                        &vg_ParamEqt[vl_NbEqt].NbCarPref,
                                        &vg_ParamEqt[vl_NbEqt].NbCarSuff ) == 5 );
             {
                 vl_NbEqt++;
             }
         }
               			
      }
      /*A Fermeture du fichier  XDF_ParamLS */
      XZSS_13FermerFichier( vl_Fd );
      /*A Retourne compte rendu OK */
      return ( vl_ValRet );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_ouv_ferm_fichier_config ( 	EINI_MACHINE *	pa_Machine,
					int va_IndiceFichier,
					int va_OptOuvFerm )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Ecrire les parametres dans le fichier de config
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
char 		pl_NomFichier[XDC_PATH_ABS_SIZE];


    if ( ( pg_TabConf[va_IndiceFichier].TypeFichier1 == EINI_TYPE_CONF ) &&
         ( strlen (pg_TabConf[va_IndiceFichier].NomFichier1) > 0 ) )
    {
       /* Suppression de l'ancien fichier */
       sprintf ( pl_NomFichier, "%s/%s_%s", 
               			XZEXC_PATH_DIFFUSION, 
               			pg_TabConf[va_IndiceFichier].NomFichier1,
               			pa_Machine->NomMachine );
       /* Si option ouverture fichier */
       if ( va_OptOuvFerm == EINI_OUVERTURE_FIC )
       {
          /* Alors creation du nouveau fichier de config */
          XZSS_06DetruireFichier ( pl_NomFichier );
          if ( ( vg_FdConf[0] = open ( pl_NomFichier, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
          {
             XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
          }
       }
       else
       {
          /* Sinon fermeture du fichier de config */
          close ( vg_FdConf[0] );
          /* Si option detruire fichier */
          if ( va_OptOuvFerm == EINI_DETRUIRE_FIC )
          {
              /* Alors detruire fichier */
              XZSS_06DetruireFichier ( pl_NomFichier );
          }
       }  
    }
    
    if ( ( pg_TabConf[va_IndiceFichier].TypeFichier2 == EINI_TYPE_CONF ) &&
         ( strlen (pg_TabConf[va_IndiceFichier].NomFichier2) > 0 ) )
    {
        /* Suppression de l'ancien fichier */
        sprintf ( pl_NomFichier, "%s/%s_%s", 
               			 XZEXC_PATH_DIFFUSION, 
               			 pg_TabConf[va_IndiceFichier].NomFichier2,
               			 pa_Machine->NomMachine );
       /* Si option ouverture fichier */
       if ( va_OptOuvFerm == EINI_OUVERTURE_FIC )
       {
          /* Alors creation du nouveau fichier de config */
          XZSS_06DetruireFichier ( pl_NomFichier );
		  XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture fichier %s", pl_NomFichier ); 
          if ( (vg_FdConf[1] = open ( pl_NomFichier, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0)
          {
             XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
          }
       }
       else
       {
          /* Sinon fermeture du fichier de config */
          close ( vg_FdConf[1] );
          /* Si option detruire fichier */
          if ( va_OptOuvFerm == EINI_DETRUIRE_FIC )
          {
              /* Alors detruire fichier */
              XZSS_06DetruireFichier ( pl_NomFichier );
          }
       }  
    }
    
    if ( ( pg_TabConf[va_IndiceFichier].TypeFichier3 == EINI_TYPE_CONF ) &&
         ( strlen (pg_TabConf[va_IndiceFichier].NomFichier3) > 0 ) )
    {
        /* Suppression de l'ancien fichier */
        sprintf ( pl_NomFichier, "%s/%s_%s", 
               			 XZEXC_PATH_DIFFUSION, 
               			 pg_TabConf[va_IndiceFichier].NomFichier3,
               			 pa_Machine->NomMachine );
       /* Si option ouverture fichier */
       if ( va_OptOuvFerm == EINI_OUVERTURE_FIC )
       {
          XZSS_06DetruireFichier ( pl_NomFichier );
          /* Alors creation du nouveau fichier de config */
		  XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture fichier %s", pl_NomFichier ); 
          if ( ( vg_FdConf[2] = open ( pl_NomFichier, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
          {
             XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
          }
       }
       else
       {
          /* Sinon fermeture du fichier de config */
          close ( vg_FdConf[2] );
          /* Si option detruire fichier */
          if ( va_OptOuvFerm == EINI_DETRUIRE_FIC )
          {
              /* Alors detruire fichier */
              XZSS_06DetruireFichier ( pl_NomFichier );
          }
       }  

     }
     
	 if ( ( pg_TabConf[va_IndiceFichier].TypeFichier4 == EINI_TYPE_CONF ) &&
         ( strlen (pg_TabConf[va_IndiceFichier].NomFichier4) > 0 ) )
    {
        /* Suppression de l'ancien fichier */
        sprintf ( pl_NomFichier, "%s/%s_%s", 
               			 XZEXC_PATH_DIFFUSION, 
               			 pg_TabConf[va_IndiceFichier].NomFichier4,
               			 pa_Machine->NomMachine );
       /* Si option ouverture fichier */
       if ( va_OptOuvFerm == EINI_OUVERTURE_FIC )
       {
          XZSS_06DetruireFichier ( pl_NomFichier );
          /* Alors creation du nouveau fichier de config */
		  XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture fichier %s", pl_NomFichier ); 
          if ( ( vg_FdConf[3] = open ( pl_NomFichier, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
          {
             XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
          }
       }
       else
       {
          /* Sinon fermeture du fichier de config */
          close ( vg_FdConf[3] );
          /* Si option detruire fichier */
          if ( va_OptOuvFerm == EINI_DETRUIRE_FIC )
          {
              /* Alors detruire fichier */
              XZSS_06DetruireFichier ( pl_NomFichier );
          }
       }  

     }
	 
	if ( ( pg_TabConf[va_IndiceFichier].TypeFichier5 == EINI_TYPE_CONF ) &&
         ( strlen (pg_TabConf[va_IndiceFichier].NomFichier5) > 0 ) )
    {
        /* Suppression de l'ancien fichier */
        sprintf ( pl_NomFichier, "%s/%s_%s", 
               			 XZEXC_PATH_DIFFUSION, 
               			 pg_TabConf[va_IndiceFichier].NomFichier5,
               			 pa_Machine->NomMachine );
       /* Si option ouverture fichier */
       if ( va_OptOuvFerm == EINI_OUVERTURE_FIC )
       {
          XZSS_06DetruireFichier ( pl_NomFichier );
          /* Alors creation du nouveau fichier de config */
		  XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture fichier %s", pl_NomFichier ); 
          if ( ( vg_FdConf[4] = open ( pl_NomFichier, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
          {
             XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
          }
       }
       else
       {
          /* Sinon fermeture du fichier de config */
          close ( vg_FdConf[4] );
          /* Si option detruire fichier */
          if ( va_OptOuvFerm == EINI_DETRUIRE_FIC )
          {
              /* Alors detruire fichier */
              XZSS_06DetruireFichier ( pl_NomFichier );
          }
       }  

     }

	 
	if ( ( pg_TabConf[va_IndiceFichier].TypeFichier6 == EINI_TYPE_CONF ) &&
         ( strlen (pg_TabConf[va_IndiceFichier].NomFichier6) > 0 ) )
    {
        /* Suppression de l'ancien fichier */
        sprintf ( pl_NomFichier, "%s/%s_%s", 
               			 XZEXC_PATH_DIFFUSION, 
               			 pg_TabConf[va_IndiceFichier].NomFichier6,
               			 pa_Machine->NomMachine );
       /* Si option ouverture fichier */
       if ( va_OptOuvFerm == EINI_OUVERTURE_FIC )
       {
          XZSS_06DetruireFichier ( pl_NomFichier );
          /* Alors creation du nouveau fichier de config */
		  XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture fichier %s", pl_NomFichier ); 
          if ( ( vg_FdConf[5] = open ( pl_NomFichier, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
          {
             XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
          }
       }
       else
       {
          /* Sinon fermeture du fichier de config */
          close ( vg_FdConf[5] );
          /* Si option detruire fichier */
          if ( va_OptOuvFerm == EINI_DETRUIRE_FIC )
          {
              /* Alors detruire fichier */
              XZSS_06DetruireFichier ( pl_NomFichier );
          }
       }  

     }	 
}

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_write_fic ( 	XDY_Fichier	va_Fd,
			char *		pa_TexteFile,
			int		va_LgTexte,
			char *		pa_NomFic )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Ecriture d'une chaine de caractere dans une fichier prealablement ouvert 
*
------------------------------------------------------*/
{

    if ( va_Fd < 0 ) return ( XDC_OK);

    if ( write ( va_Fd, pa_TexteFile, va_LgTexte ) < 0 )
    {
       XZST_03EcritureTrace ( XZSTC_WARNING, " Impossible d'ecrire dans le fichier %s_%s.",
       					     pa_NomFic, pg_MachineCrt->NomMachine ); 
       return ( XDC_NOK);
    }
    else
    {
       return ( XDC_OK);
    }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_mise_a_jour_config_ligne ( 	int		va_NumLiaison,
					char *		pa_NomServeur,
					int		va_Protocole,
					int		va_Sens,
					XDY_Eqt		va_TypeEqt,
					XDY_Eqt		va_NumEqt ,
					int	va_vitesse)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Ecrire les parametres dans le fichier de config
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
int 		vl_Index = 0;
int vl_indice = 0;
int 		vl_NumLiaison = 0;
XDY_NomEqt	vl_NomServeur = "";
XDY_Booleen	vb_9600 = XDC_FAUX;
XDY_PR		vl_PR=0;
char		vl_site[10]="";
char		vt_PR[10]="";


      /*A Appel de ec_mise_a_jour_lcr pour fichier fenetre LCR */
      if ((va_Protocole != XZECC_PROT_DIASER) && (va_Protocole != XZECC_PROT_OPC))
	  {
      	if ( ec_mise_a_jour_lcr ( va_TypeEqt, va_NumEqt ) == XDC_NOK ) return;
	}

      if ((va_Protocole== XZECC_PROT_DIASER) || (va_Protocole== XZECC_PROT_IP)
         || (va_Protocole== XZECC_PROT_OPC))  {
         XZST_03EcritureTrace( XZSTC_WARNING, "Config de la ligne IP %s type eqt : %d",
	 		pa_NomServeur, va_TypeEqt);
	if (va_Sens==XZECC_SENS_MAITRE)
  		vl_Index = EINI_MAX_LIGNE_PAR_EQT -1; /*XZECC_NUM_IP_MAITRE*/
     	else
  		vl_Index = EINI_MAX_LIGNE_PAR_EQT -2; /*XZECC_NUM_IP_ESCLAVE*/

   	strcpy ( pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].NumeroST,pa_NomServeur);
	XZST_03EcritureTrace( XZSTC_WARNING, "NumeroST = %s",pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].NumeroST);
	sprintf (pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].NumLigne, "%d", va_TypeEqt);
	pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].Protocole = va_Protocole;
	pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].Sens      = va_Sens;
	/* Rechercher le premier index dispo en partant de la fin */
	for (vl_Index=XZECC_NB_LIGNE_SERIE;vl_Index >=0; vl_Index--)
		/* on verifie que le type d'equipement n'est pas deja present ou que l'emplacement est libre */
		if ((!strcmp(pg_MachineCrt->LigneSerie[vl_Index].NumeroST,"")) || (atoi(pg_MachineCrt->LigneSerie[vl_Index].NumLigne)==va_TypeEqt))
			break;
	sprintf(pg_MachineCrt->LigneSerie[vl_Index].NumLigne , "%d", va_TypeEqt);
	strcpy(pg_MachineCrt->LigneSerie[vl_Index].NumeroST,pa_NomServeur);
	pg_MachineCrt->LigneSerie[vl_Index].Protocole = va_Protocole;
	pg_MachineCrt->LigneSerie[vl_Index].Sens = va_Sens;
	sprintf(pg_MachineCrt->LigneSerie[vl_Index].Vitesse  ,"%d", 1);
	 for ( vl_indice = 0; vl_indice < XZECC_MAX_EQT; vl_indice++)
	 {
	    if ( !strcmp ( pg_MachineCrt->Eqt[ec_rang_eqt(va_TypeEqt)].Equipement, vg_ParamEqt[vl_indice].Equipement ) )
             {
			pg_MachineCrt->LigneSerie[vl_Index].NbEssais = vg_ParamEqt[vl_indice].NbEssais;
			pg_MachineCrt->LigneSerie[vl_Index].NbCarPref = vg_ParamEqt[vl_indice].NbCarPref;
			pg_MachineCrt->LigneSerie[vl_Index].NbCarSuff = vg_ParamEqt[vl_indice].NbCarSuff;
		break;
	 	}
	  }
        return;
      }

      vl_Index = 0;
      /*A Test si il y a deux serveur sur la machine */
      sscanf ( pa_NomServeur, "%s", vl_NomServeur );

	// Si serveur est un POLT ou un ST ou un LT...
	if (strlen(vl_NomServeur) >= 7)
	{
		strncpy (vt_PR, &vl_NomServeur[strlen(vl_NomServeur )-3], 3);
		strncpy (vl_site, &vl_NomServeur[strlen(vl_NomServeur )-5], 2);
		vl_PR=atoi (vt_PR);		
	}
	else
	{
		strncpy (vt_PR, "0", 1);
		strncpy (vl_site, &vl_NomServeur[strlen(vl_NomServeur )-2], 2);
		vl_PR=0;
	}

 XZST_03EcritureTrace( XZSTC_WARNING, "Config de la ligne serie %s No:%d ; site=%s;PR=%d ; vitesse = %d",vl_NomServeur,va_NumLiaison,vl_site,vl_PR, va_vitesse);

      if ( strlen ( pg_MachineCrt->NumeroSTA ) == 0 )
      {
         strcpy ( pg_MachineCrt->NumeroSTA, vl_NomServeur );
      }
      else
      {
         if ( strcmp ( pg_MachineCrt->NumeroSTA, vl_NomServeur ) != 0 )
         { 
            if ( strlen ( pg_MachineCrt->NumeroSTB ) == 0 )
            {
               strcpy ( pg_MachineCrt->NumeroSTB, vl_NomServeur );
            }
            else
            {
               /* Envoi message a l'administrateur systeme */
               /* Utilisation de XZST_10ArchiverMsgSyst ( ... ); */
               XZST_03EcritureTrace( XZSTC_WARNING, "Config serveur %s sur %s incorrect", 
         					     vl_NomServeur,
         					     pg_MachineCrt->NomMachine );
            }
         }
      }
       
      vl_NumLiaison = ( !strcmp ( pg_MachineCrt->NumeroSTA, vl_NomServeur ) ) ? va_NumLiaison : va_NumLiaison +10;
      
      /* Controle si la ligne n'est pas deja utilisee par un autre protocole */
      if ( ( strlen (pg_MachineCrt->LigneSerie[vl_NumLiaison].NumeroST) > 0 ) &&
           ( pg_MachineCrt->LigneSerie[vl_NumLiaison].Protocole != va_Protocole ) &&
           ( pg_MachineCrt->LigneSerie[vl_NumLiaison].Sens != va_Sens ) )
      {
         /* Envoi message a l'administrateur systeme */
         /* Utilisation de XZST_10ArchiverMsgSyst ( ... ); */
         XZST_03EcritureTrace( XZSTC_WARNING, "Config de la ligne serie No:%d incorrecte", vl_NumLiaison );
      }
      else
      {
          /* Mise a jour de la configuration de la ligne */
          strcpy ( pg_MachineCrt->LigneSerie[vl_NumLiaison].NumeroST, vl_NomServeur );
          sprintf (pg_MachineCrt->LigneSerie[vl_NumLiaison].NumLigne, "%d", vl_NumLiaison );
          pg_MachineCrt->LigneSerie[vl_NumLiaison].Protocole = va_Protocole;
          pg_MachineCrt->LigneSerie[vl_NumLiaison].Sens      = va_Sens;
          
          for ( vl_Index = 0; vl_Index < XZECC_MAX_EQT; vl_Index++)
          {
	     if ( !strcmp ( pg_MachineCrt->Eqt[ec_rang_eqt(va_TypeEqt)].Equipement, vg_ParamEqt[vl_Index].Equipement ) )
             {
          	strcpy ( pg_MachineCrt->LigneSerie[vl_NumLiaison].Vitesse, vg_ParamEqt[vl_Index].Vitesse );
          	pg_MachineCrt->LigneSerie[vl_NumLiaison].NbEssais 	= vg_ParamEqt[vl_Index].NbEssais;
          	pg_MachineCrt->LigneSerie[vl_NumLiaison].NbCarPref 	= vg_ParamEqt[vl_Index].NbCarPref;
          	pg_MachineCrt->LigneSerie[vl_NumLiaison].NbCarSuff 	= vg_ParamEqt[vl_Index].NbCarSuff;
		if ( !strcmp (vg_ParamEqt[vl_Index].Equipement,"rdt"))
		{
			if ((strcmp (vl_site,"CA")&& (strcmp (vl_site,"VC"))) || (vl_PR < 159) )
				strcpy ( pg_MachineCrt->LigneSerie[vl_NumLiaison].Vitesse, "1200");
			else
				strcpy ( pg_MachineCrt->LigneSerie[vl_NumLiaison].Vitesse, "9600");

			if ((!strcmp(vl_site,"DP")) && ((vl_PR==72) || (vl_PR==6)))
				strcpy ( pg_MachineCrt->LigneSerie[vl_NumLiaison].Vitesse, "9600");
		}

		if ( !strcmp (vg_ParamEqt[vl_Index].Equipement,"pmv"))
		{
			if (va_vitesse==0)
			{
				strcpy ( pg_MachineCrt->LigneSerie[vl_NumLiaison].Vitesse, vg_ParamEqt[vl_Index].Vitesse );
			} else
			{
				sprintf(pg_MachineCrt->LigneSerie[vl_NumLiaison].Vitesse,"%d",va_vitesse);
			}
			if ((!strcmp(vl_site,"DP")) && ((vl_PR==72) || (vl_PR==6)))
				strcpy ( pg_MachineCrt->LigneSerie[vl_NumLiaison].Vitesse, "9600");
                }
          		break;
	    }
          }
          
          for ( vl_Index = 0; vl_Index < EINI_MAX_LIGNE_PAR_EQT; vl_Index++)
          {
             if ( ( !strcmp (pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].NumeroST, vl_NomServeur) ) &&
                  ( atoi(pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].NumLigne) == vl_NumLiaison ) )
                break;
             if ( atoi(pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].NumLigne) == 0 )
             {
                strcpy ( pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].NumeroST, vl_NomServeur );
                sprintf (pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].NumLigne, "%d", vl_NumLiaison );
                pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].Protocole = va_Protocole;
                pg_MachineCrt->LigneEqt[ec_rang_eqt (va_TypeEqt)].Ligne[vl_Index].Sens      = va_Sens;

                break;
             }
          }             
          
      }
}


/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Appel du module de MALL et des modules de Mzzz selon le message XDM_ECONFIG_EQPT.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config ( XDY_NomMachine	pa_NomMachine, int va_TypeEqt )

/*
* ARGUMENTS EN ENTREE :
*  XDY_NomMachine	pa_NomMachine	: Nom de la machine (une ou toutes)
*  int 			va_TypeEqt	: Type d'equipement concernee (un ou tous)

*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelÅe par ed_config_cb et au lancement de la tache TEINI
*
* FONCTION 
*  Appel de MALL et appel sequentiel des modules Mzzz selon le message specifie
*  Appel de MSYN en donnant le nom de la machine (une ou toutes) et le type de configuration
*
------------------------------------------------------*/
{
	EINI_MACHINE *	pl_Machine;
	char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
	XDY_Fichier		vl_Fd;
	int				vl_IndexEqt 	= 0,
					vl_IndexLigne	= 0,
					vl_Index		= 0,
					vl_PresenceEqt  = 0,
					vl_ptr			= 0;
	XDY_Texte		pl_TexteConf;
	char			vl_LigneSerie[16];
	XDY_Datagroup	pl_NomDG;

	XZST_03EcritureTrace(XZSTC_WARNING, "IN: ed_config ");

	/* Ne pas enlever le sleep : sans ce sleep, provoque une erreur -4 sur ct_cmd_alloc (CS_BUSY) */
	XZST_03EcritureTrace(XZSTC_WARNING, "Attente 10 secondes pour eviter CS_BUSY sur ct_cmd_alloc." );
	sleep(10);

	/* Recopie des fichiers standard de configuration */
	ei_creation_fich_utilisateurs ();

	/* Creation du fichiers des utilisateurs */
	ei_diffusion_fichier_standard ();

	/* Lecture du fichier des parametre des lignes series */
	ec_lire_parametre_LS	();

	/* Appel du module MALL */
	ec_config_all ( pa_NomMachine, va_TypeEqt );

	/* Recherche de la machine concerne */
	for ( pl_Machine = pg_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant )
	{
		/* S'il s'agit de la machine concernee */
		if ( ( !strcmp (pl_Machine->NomMachine, pa_NomMachine) ) ||
		     ( !strcmp (pa_NomMachine, XZECC_TOUTES_MACHINES) ) )
		{
			/* Creation du nouveau fichier de XDF_Config_Lignes et XDF_Presence_Equip */
			/* Construction du nom du fichier presence equipement */
			sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_DIFFUSION, XDF_Presence_Equip, pl_Machine->NomMachine );
			/* Ouverture du fichier presence equipement apres avoir detruit l'ancien*/
			XZSS_06DetruireFichier ( pl_PathEtFic );
			vl_PresenceEqt = 0;
			if ( (vl_Fd = open ( pl_PathEtFic, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) >= 0 )
			{
				/* Ecriture dans le fichier XDF_Presence_Equip de la config de chaque equipements */
				for ( vl_IndexEqt = 0; vl_IndexEqt < pg_SizeTabConf; vl_IndexEqt++)
				{
					if ( strlen (pl_Machine->Eqt[vl_IndexEqt].Equipement) > 0)
					{
						vl_ptr = sprintf ( pl_TexteConf, 	" %s %d %d ", 
										pl_Machine->Eqt[vl_IndexEqt].Equipement, 
										pl_Machine->Eqt[vl_IndexEqt].Present,
										pl_Machine->Eqt[vl_IndexEqt].Type );

						for ( vl_Index = 0; vl_Index < EINI_MAX_LIGNE_PAR_EQT; vl_Index++)
						{
							if ( atoi(pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].NumLigne) > 0 )
							{
								if ((pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].Protocole == XZECC_PROT_DIASER) ||
				  					(pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].Protocole == XZECC_PROT_IP))
								{
									sprintf ( vl_LigneSerie, "%s", pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].NumLigne);
								}
								else
									sprintf ( vl_LigneSerie, "%s_%d", 
				  		  					pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].NumLigne, 
				  		  					XZECC_ADRESSE_PORT_ST + atoi (pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].NumLigne) );
				            	

								vl_ptr += sprintf ( pl_TexteConf+vl_ptr, "$ %s %s %c %c", 
													pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].NumeroST,
													vl_LigneSerie,
													pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].Protocole,
													pl_Machine->LigneEqt[vl_IndexEqt].Ligne[vl_Index].Sens );

							}
						}
						if ( pl_Machine->Eqt[vl_IndexEqt].Present ) 
							vl_PresenceEqt++;    

						vl_ptr += sprintf ( pl_TexteConf+vl_ptr, " \n\0");

						write ( vl_Fd, pl_TexteConf, strlen (pl_TexteConf) );
						
					}
				}
				/* Fermeture du fichier XDF_Presence_Equip */
				close ( vl_Fd );
			}
			else
			{
			    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_PathEtFic );
			}

			if ( vl_PresenceEqt == 0 )
			{
			    XZSS_06DetruireFichier ( pl_PathEtFic );
			}
			else
			{
			   /* Construction du nom du fichier XDF_Config_Lignes */
			   sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_DIFFUSION, XDF_Config_Lignes, pl_Machine->NomMachine );

			   /* Ouverture du fichier XDF_Config_Lignes apres avoir detruit l'ancien */
			   XZSS_06DetruireFichier ( pl_PathEtFic );
			   if ( (vl_Fd = open ( pl_PathEtFic, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) >= 0 )
			   {
			      /* Ecriture dans le fichier XDF_Config_Lignes de la config de chaque lignes series */
			      for ( vl_IndexLigne = 0; vl_IndexLigne <= XZECC_NB_LIGNE_SERIE; vl_IndexLigne++)
			      {
			if ( ( strlen (pl_Machine->LigneSerie[vl_IndexLigne].NumeroST) > 0 ) &&
			              ( atoi (pl_Machine->LigneSerie[vl_IndexLigne].NumLigne) > 0 )/* &&
			              ( atoi (pl_Machine->LigneSerie[vl_IndexLigne].NumLigne) <= XZECC_NB_LIGNE_SERIE )*/ )
			         {
			        sprintf ( vl_LigneSerie, 	"%s_%d", 
			     				pl_Machine->LigneSerie[vl_IndexLigne].NumLigne, 
			     				XZECC_ADRESSE_PORT_ST + atoi (pl_Machine->LigneSerie[vl_IndexLigne].NumLigne) );
			            sprintf ( pl_TexteConf, " %s\t%s\t%c\t%c\t%s\t%d\t%d\t%d\n\0", 
			                                  pl_Machine->LigneSerie[vl_IndexLigne].NumeroST,
			                                  vl_LigneSerie,
			                                  pl_Machine->LigneSerie[vl_IndexLigne].Protocole,
			                                  pl_Machine->LigneSerie[vl_IndexLigne].Sens,
			                                  pl_Machine->LigneSerie[vl_IndexLigne].Vitesse,
			                                  pl_Machine->LigneSerie[vl_IndexLigne].NbEssais,
			                                  pl_Machine->LigneSerie[vl_IndexLigne].NbCarPref,
			                                  pl_Machine->LigneSerie[vl_IndexLigne].NbCarSuff );
			            write ( vl_Fd, pl_TexteConf, strlen (pl_TexteConf) );
			            XZST_03EcritureTrace ( XZSTC_DEBUG1, " Ligne No: %s : %s",pl_Machine->LigneSerie[vl_IndexLigne].NumLigne, pl_TexteConf );
			         }
			      }
			      /*A Fermeture du fichier XDF_Config_Lignes */
			      close ( vl_Fd );
			   }
			   else
			   {
			       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_PathEtFic );
			   }
			}
		}
	}
   
	/*A Envoi demande de reconfig fichier LCR a TEINI du CI ou CA */
	if (vg_SitePrincipal != XDC_CI)
	{
   		XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT , XDC_NOM_SITE_CA );
	}
	else
	{
   		XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT , XDC_NOM_SITE_CI );
	}

	if (!TipcSrvMsgWrite (  pl_NomDG
								,XDM_IdentMsg(XDM_EINIT)
								,XDM_FLG_SRVMSGWRITE
								,T_IPC_FT_STR,pa_NomMachine
								,T_IPC_FT_INT4,va_TypeEqt
								,T_IPC_FT_INT4,XZEXC_RECONFIG_LCR
								,NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EINIT non effectue." );
	}

   /*A Cretion des fichiers pour fenetre LCR */
/*!!   ec_fichier_lcr ();  */
   
   /*A Diffusion des fichiers */
   if ( vg_InitTache == XDC_OK )
   {
      ec_diffusion_fichier ( pa_NomMachine, XZECC_MACHINE_PRINC, va_TypeEqt );
   }
   else
   {
      vg_InitTache = XDC_OK;
   }

   /*A Configuration des DataGroup de surveillance */
   ec_config_surveillance ( pa_NomMachine, va_TypeEqt );

   
} 





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config_all ( XDY_NomMachine	pa_NomMachine, int va_TypeEqt )

/*
* ARGUMENTS EN ENTREE :
*  XDY_NomMachine	pa_NomMachine	: Nom de la machine (une ou toutes)
*  int 			va_TypeEqt	: Type d'equipement concernee (un ou tous)
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR : 
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION 
*   Recherche de la liste des machines du site concerne
*   Pour chaque machine initialise les fichier de config (XDF_Config_Ligne et 
*   XDF_Presence_Equip)
*
------------------------------------------------------*/
{
EINI_MACHINE 	*pl_Machine,
		*pl_SupMachine,
		*pl_PreMachine;
int		vl_NbMachine = 0,
		vl_ValRet = XDC_OK;
XDY_NomSite	pl_NomSite;
		
   /* Recherche de la machine concerne */
   pl_PreMachine = pl_Machine = pg_ListeMachine;
   while ( pl_Machine != NULL)
   {
      /* S'il s'agit de tous les equipements et s'il s'agit de la machine concernee */
      if ( ( va_TypeEqt == XZECC_TYPE_EQT_ALL ) &&
           ( ( !strcmp ( pl_Machine->NomMachine, pa_NomMachine ) ) ||
             ( !strcmp ( pa_NomMachine, XZECC_TOUTES_MACHINES ) ) ) )
      {
         pl_SupMachine = pl_Machine;
         if ( (pl_PreMachine == pg_ListeMachine) && (pl_PreMachine == pl_Machine) )
         {
            pg_ListeMachine = pl_Machine->Suivant;
            pl_PreMachine   = pl_Machine->Suivant;
         }
         else
         {
            pl_PreMachine->Suivant = pl_Machine->Suivant;
         }
         pl_Machine = pl_Machine->Suivant;

         /* Suppression de la machine concernee dans la liste des machines exitantes */
         free ( pl_SupMachine);
      }
      else
      {
         pl_PreMachine = pl_Machine;
         pl_Machine = pl_Machine->Suivant;
      }
   }
   
   /*A Recuperation du site de la machine */
   if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
   } 
   
   /* Recherche la liste de toutes les machines (appel de XZSA_19) */
   XZSA_19ListeMachineSite ( 	pl_NomSite,
   				(XDY_FonctionInt)ec_liste_machine, 
   				&vl_NbMachine, 
   				NULL );
   /* Recherche des types de toutes les machines (appel de XZSA_17) */
   for ( pl_PreMachine = pl_Machine = pg_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant)
      XZSA_17TypeMachine ( pl_Machine->NomMachine, &(pl_Machine->TypeMachine) );
   
   /* Configuration des equipements pour fenetre LCR */
   ec_config_lcr ( va_TypeEqt );
   
   /* Configuration des equipements */
   ec_config_eqt ( pa_NomMachine, va_TypeEqt );

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config_eqt (		XDY_NomMachine	pa_NomMachine,
				int		va_NumEqt )

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine
*   int			va_NumEqt
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Detruire les anciens fichiers
*   Creer les fichiers de config
*   Lire en base de donnees les parametres de configuration du type d'equipement concerne
*   Ecrire les donnees dans le fichier de configuration
*   Fermer les fichiers de configuration
*
------------------------------------------------------*/
{
  EINI_MACHINE *	pl_Machine; 
  EINI_MACHINE *	pl_MachineSur;
  XDY_NomMachine	vl_NomMachineSup = "";
  int			vl_indice = 0,
			vl_ValRet = XDC_OK,
			vl_EqtOk  = 0;
  char          	pl_PathEtFic[XDC_PATH_ABS_SIZE];
  XDY_Texte		pl_TexteConf;
  int			vl_Fd = 0;
  
   /* Recherche de la machine concerne */
   for ( pl_Machine = pg_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant )
   {
      /* S'il s'agit de la machine concernee */
      if ( ((pl_Machine->TypeMachine == XZSCC_TYPEM_SD) || (pl_Machine->TypeMachine == XZSCC_TYPEM_SC) ||
	    (pl_Machine->TypeMachine == XZSCC_TYPEM_SP) || (pl_Machine->TypeMachine == XZSCC_TYPEM_LT) ||
	    (pl_Machine->TypeMachine == XZSCC_TYPEM_PC2) || (pl_Machine->TypeMachine == XZSCC_TYPEM_PC2LT)) &&
           ( !strcmp (pl_Machine->NomMachine, pa_NomMachine) ||
             !strcmp (pa_NomMachine, XZECC_TOUTES_MACHINES)) ) 
      {
         /* Mise a jour de la variable globale Nom de la machine concerne */
         pg_MachineCrt = pl_Machine;
         
         /* Recherche de l'equipement concerne */
         for ( vl_indice = 0; vl_indice < pg_SizeTabConf; vl_indice++)
         {
            /* S'il s'agit de l'equipement concerne */
            if ( ( pg_TabConf[vl_indice].TypeEqt == va_NumEqt ) || ( va_NumEqt == XZECC_TYPE_EQT_ALL ) ||
                 ( (va_NumEqt == XDC_EQT_PMV) && (pg_TabConf[vl_indice].TypeEqt == XDC_EQT_PCT) ) )
            {
               ec_ouv_ferm_fichier_config ( pl_Machine, vl_indice, EINI_OUVERTURE_FIC );
               vl_EqtOk = 1;
               ed_reception ();
            }
            else
               vl_EqtOk = 0;
            
            /* Selon le type d'equipement */
            switch ( pg_TabConf[vl_indice].TypeEqt )
            {
               case XZECC_TYPE_EQT_RAU :
                  /* Appel de la fonction ei_config_eqt_rau du module MRAU */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RAU)].Type = XZECC_TYPE_EQT_RAU;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RAU)].Equipement, XZECC_TACHE_RAU );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_rau ( pl_Machine );
                  break;
                   
               case XDC_EQT_GEN_LCR :
                  /* Appel de la fonction ei_config_EQT_GEN_LCR du module MQTX */
                  pl_Machine->Eqt[ec_rang_eqt(XDC_EQT_GEN_LCR)].Type = XDC_EQT_GEN_LCR;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XDC_EQT_GEN_LCR)].Equipement, XZECC_TACHE_QTX );
                  if (vl_EqtOk) vl_ValRet = ei_config_EQT_GEN_LCR ( pl_Machine );
                  break;
                   
               case XDC_EQT_TDP :
                  /* Appel de la fonction ei_config_eqt_TDP du module MTDP */
                  pl_Machine->Eqt[ec_rang_eqt(XDC_EQT_TDP)].Type = XDC_EQT_TDP;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XDC_EQT_TDP)].Equipement, XZECC_TACHE_TDP );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_TDP ( pl_Machine );
                  break;
                   
               case XZECC_TYPE_EQT_VID :
                  /* Appel de la fonction ei_config_eqt_video du module MVID */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_VID)].Type = XZECC_TYPE_EQT_VID;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_VID)].Equipement, XZECC_TACHE_VID );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_video ( pl_Machine );
                  break;
                  
/************ MAA telephonique gere par webservice sur SPCI2 DEM965
               case XZECC_TYPE_EQT_TEL :
                  // Appel de la fonction ei_config_eqt_maa du module MTEL
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_TEL)].Type = XZECC_TYPE_EQT_TEL;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_TEL)].Equipement, XZECC_TACHE_TEL );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_maa ( pl_Machine );
                  break;
**************/
                  
               case XZECC_TYPE_EQT_RDT :
                  /* Appel de la fonction ei_config_eqt_radt du module MRDT */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RDT)].Type = XZECC_TYPE_EQT_RDT;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RDT)].Equipement, XZECC_TACHE_RDT );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_radt ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_DAI :
                  /* Appel de la fonction ei_config_eqt_dai du module MDAI */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_DAI)].Type = XZECC_TYPE_EQT_DAI;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_DAI)].Equipement, XZECC_TACHE_DAI );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_dai ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_PMV :
                  /* Appel de la fonction ei_config_eqt_pmv du module MPMV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Type = XZECC_TYPE_EQT_PMV;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Equipement, XZECC_TACHE_PMV );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_pmv ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_PMVA :
                  /* Appel de la fonction ei_config_eqt_pmv du module MPMV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMVA)].Type = XZECC_TYPE_EQT_PMVA;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMVA)].Equipement, XZECC_TACHE_PMVA );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_pmva ( pl_Machine );
                  break;
			
			   case XZECC_TYPE_EQT_PRV :
                  /* Appel de la fonction ei_config_eqt_pmv du module MPRV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PRV)].Type = XZECC_TYPE_EQT_PRV;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PRV)].Equipement, XZECC_TACHE_PRV );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_prv ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_PICTO :
                  /* Appel de la fonction ei_config_eqt_picto du module MPMV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Type = XZECC_TYPE_EQT_PMV;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Equipement, XZECC_TACHE_PMV );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_picto ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_NAV :
                  /* Appel de la fonction ei_config_eqt_nav du module MNAV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NAV)].Type = XZECC_TYPE_EQT_NAV;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NAV)].Equipement, XZECC_TACHE_NAV );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_nav ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_BAF :
                  /* Appel de la fonction ei_config_eqt_baf du module MNAV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAF)].Type = XZECC_TYPE_EQT_BAF;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAF)].Equipement, XZECC_TACHE_BAF );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_baf ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_BAD :
                  /* Appel de la fonction ei_config_eqt_bad du module MNAV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAD)].Type = XZECC_TYPE_EQT_BAD;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAD)].Equipement, XZECC_TACHE_BAD );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_bad ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_CFE :
                  /* Appel de la fonction ei_config_eqt_pal du module MPAL */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CFE)].Type = XZECC_TYPE_EQT_CFE;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CFE)].Equipement, XZECC_TACHE_CFE );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_cfe ( pl_Machine );
                  break;

	       case XZECC_TYPE_EQT_CAP:
		  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CAP)].Type = XZECC_TYPE_EQT_CAP;
		  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CAP)].Equipement, XZECC_TACHE_CAP);
		  if (vl_EqtOk) vl_ValRet = ei_config_eqt_cap ( pl_Machine );
		  break;

               case XZECC_TYPE_EQT_PAL :
                  /* Appel de la fonction ei_config_eqt_pal du module MPAL */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PAL)].Type = XZECC_TYPE_EQT_PAL;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PAL)].Equipement, XZECC_TACHE_PAL );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_pal ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_IMU :
                  /* Appel de la fonction ei_config_eqt_imu du module IMU */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_IMU)].Type = XZECC_TYPE_EQT_IMU;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_IMU)].Equipement, XZECC_TACHE_IMU );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_imu ( pl_Machine );
                  break;
               case XZECC_TYPE_EQT_GTC :
                  /* Appel de la fonction ei_config_eqt_MiniGTC du module MGTC */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_GTC)].Type = XZECC_TYPE_EQT_GTC;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_GTC)].Equipement, XZECC_TACHE_GTC );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_MiniGTC ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_NIC :
                  /* Appel de la fonction ei_config_eqt_GTC_NICE du module MGTC */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Type = XZECC_TYPE_EQT_NIC;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Equipement, XZECC_TACHE_NIC );
                  if (vl_EqtOk)
				  {
					/*  ABE 27-04-2015 : projet suppression ancienne GTC (GTCA) */
					// vl_ValRet = ei_config_eqt_GTC_NICE ( pl_Machine );
					vl_ValRet = ei_config_eqt_GTC_NICE_RENOVEE ( pl_Machine );
				  }
                  break;
                  
               case XZECC_TYPE_EQT_OPE :
                  /* Mise a jour nom tache */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_OPE)].Type = XZECC_TYPE_EQT_OPE;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_OPE)].Equipement, XZECC_TACHE_OPE );
                  if (vl_EqtOk) vl_ValRet = XDC_NOK;
                  break;

			   case XZECC_TYPE_EQT_PIC :
                  /* Appel de la fonction ei_config_eqt_pic du module PIC */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PIC)].Type = XZECC_TYPE_EQT_PIC;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PIC)].Equipement, XZECC_TACHE_PIC );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_pic ( pl_Machine );
                  break;

               default: 
                  vl_ValRet = XDC_NOK;
                  break;
            }

            /* Fermeture des fichiers de configuration */
            /* S'il s'agit de l'equipement concerne */
            if ( ( pg_TabConf[vl_indice].TypeEqt == va_NumEqt ) || ( va_NumEqt == XZECC_TYPE_EQT_ALL ) ||
                 ( (va_NumEqt == XDC_EQT_PMV) && (pg_TabConf[vl_indice].TypeEqt == XDC_EQT_PCT) ) )
            {               
               if ( vl_ValRet == XDC_OK )
                   ec_ouv_ferm_fichier_config ( pl_Machine, vl_indice, EINI_FERMETURE_FIC );
               else
                   ec_ouv_ferm_fichier_config ( pl_Machine, vl_indice, EINI_DETRUIRE_FIC );
            }
            
            if ( ((pg_TabConf[vl_indice].TypeEqt == va_NumEqt) && (va_NumEqt != XDC_EQT_PMV)) ||
            	 ((pg_TabConf[vl_indice].TypeEqt == XDC_EQT_PCT) && (va_NumEqt == XDC_EQT_PMV)) ) break;
         }
            
         /*A Lecture en base du nom de la machine supleante */
         XZAO136_Lire_Suppleant ( EINI_NOM_BASE, pl_Machine->NomMachine, vl_NomMachineSup );
	 /* LINUX64 : strok */
         /*strcpy ( pl_Machine->NomMachineSup, strtok(vl_NomMachineSup, " ") );*/
          if ( vl_NomMachineSup!=(char *)NULL)
	  	strcpy ( pl_Machine->NomMachineSup, vl_NomMachineSup );
	  else
	  	strcpy ( pl_Machine->NomMachineSup, "");
      }
      
      /* S'il s'agit d'une machine PC simplifie */
      if ( ( pl_Machine->TypeMachine == XZSCC_TYPEM_PCS ) &&
           ( !strcmp (pl_Machine->NomMachine, pa_NomMachine) ||
             !strcmp (pa_NomMachine, XZECC_TOUTES_MACHINES)) ) 
      {
         /* Mise a jour de la variable globale Nom de la machine concerne */
         pg_MachineCrt = pl_Machine;
         
         /* Recherche de l'equipement concerne */
         for ( vl_indice = 0; vl_indice < pg_SizeTabConf; vl_indice++)
         {
            /* S'il s'agit de l'equipement concerne */
            if ( ( pg_TabConf[vl_indice].TypeEqt == va_NumEqt ) || ( va_NumEqt == XZECC_TYPE_EQT_ALL ) ||
                 ( (va_NumEqt == XDC_EQT_PMV) && (pg_TabConf[vl_indice].TypeEqt == XDC_EQT_PCT) ) )
            {
               ec_ouv_ferm_fichier_config ( pl_Machine, vl_indice, EINI_OUVERTURE_FIC );
               vl_EqtOk = 1;
               ed_reception ();
            }
            else
               vl_EqtOk = 0;
            
         
            /* Selon le type d'equipement */
            switch ( pg_TabConf[vl_indice].TypeEqt )
            {
               case XZECC_TYPE_EQT_GTC :
		 /* Appel de la fonction ei_config_eqt_MiniGTC du module MGTC */
		pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_GTC)].Type = XZECC_TYPE_EQT_GTC;
	     	strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_GTC)].Equipement, XZECC_TACHE_GTC );
	       if (vl_EqtOk) vl_ValRet = ei_config_eqt_MiniGTC ( pl_Machine );
		 break;
				 
	       case XZECC_TYPE_EQT_VID :
                  /* Appel de la fonction ei_config_eqt_video du module MVID */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_VID)].Type = XZECC_TYPE_EQT_VID;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_VID)].Equipement, XZECC_TACHE_VID );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_video ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_RDT :
                  /* Appel de la fonction ei_config_eqt_radt du module MRDT */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RDT)].Type = XZECC_TYPE_EQT_RDT;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RDT)].Equipement, XZECC_TACHE_RDT );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_radt ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_PMV :
                  /* Appel de la fonction ei_config_eqt_pmv du module MPMV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Type = XZECC_TYPE_EQT_PMV;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Equipement, XZECC_TACHE_PMV );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_pmv ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_PMVA :
                  /* Appel de la fonction ei_config_eqt_pmv du module MPMV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMVA)].Type = XZECC_TYPE_EQT_PMVA;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMVA)].Equipement, XZECC_TACHE_PMVA );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_pmva ( pl_Machine );
                  break;
				  
			   case XZECC_TYPE_EQT_PRV :
                  /* Appel de la fonction ei_config_eqt_pmv du module MPRV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PRV)].Type = XZECC_TYPE_EQT_PRV;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PRV)].Equipement, XZECC_TACHE_PRV );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_prv ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_PICTO :
                  /* Appel de la fonction ei_config_eqt_picto du module MPMV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Type = XZECC_TYPE_EQT_PMV;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Equipement, XZECC_TACHE_PMV );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_picto ( pl_Machine );
                  break;
                  
               case XZECC_TYPE_EQT_NAV :
                  /* Appel de la fonction ei_config_eqt_nav du module MNAV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NAV)].Type = XZECC_TYPE_EQT_NAV;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NAV)].Equipement, XZECC_TACHE_NAV );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_nav ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_BAF :
                  /* Appel de la fonction ei_config_eqt_nav du module MNAV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAF)].Type = XZECC_TYPE_EQT_BAF;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAF)].Equipement, XZECC_TACHE_BAF );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_baf ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_BAD :
                  /* Appel de la fonction ei_config_eqt_nav du module MNAV */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAD)].Type = XZECC_TYPE_EQT_BAD;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAD)].Equipement, XZECC_TACHE_BAD );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_bad ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_CFE :
                  /* Appel de la fonction ei_config_eqt_pal du module MPAL */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CFE)].Type = XZECC_TYPE_EQT_CFE;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CFE)].Equipement, XZECC_TACHE_CFE );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_cfe ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_CAP :
                  /* Appel de la fonction ei_config_eqt_pal du module MPAL */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CAP)].Type = XZECC_TYPE_EQT_CAP;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CAP)].Equipement, XZECC_TACHE_CAP );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_cap ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_PAL :
                  /* Appel de la fonction ei_config_eqt_pal du module MPAL */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PAL)].Type = XZECC_TYPE_EQT_PAL;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PAL)].Equipement, XZECC_TACHE_PAL );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_pal ( pl_Machine );
                  break;

               case XZECC_TYPE_EQT_IMU :
                  /* Appel de la fonction ei_config_eqt_imu du module IMU */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_IMU)].Type = XZECC_TYPE_EQT_IMU;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_IMU)].Equipement, XZECC_TACHE_IMU );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_imu ( pl_Machine );
                  break;

               case XDC_EQT_TDP :
                  /* Appel de la fonction ei_config_eqt_TDP du module MTDP */
                  pl_Machine->Eqt[ec_rang_eqt(XDC_EQT_TDP)].Type = XDC_EQT_TDP;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XDC_EQT_TDP)].Equipement, XZECC_TACHE_TDP );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_TDP ( pl_Machine );
                  break;
                   
               case XDC_EQT_GEN_LCR :
                  /* Appel de la fonction ei_config_EQT_GEN_LCR du module MQTX */
                  pl_Machine->Eqt[ec_rang_eqt(XDC_EQT_GEN_LCR)].Type = XDC_EQT_GEN_LCR;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XDC_EQT_GEN_LCR)].Equipement, XZECC_TACHE_QTX );
                  if (vl_EqtOk) vl_ValRet = ei_config_EQT_GEN_LCR ( pl_Machine );
                  break;

			   case XZECC_TYPE_EQT_PIC :
                  /* Appel de la fonction ei_config_eqt_pic du module PIC */
                  pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PIC)].Type = XZECC_TYPE_EQT_PIC;
                  strcpy ( pl_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PIC)].Equipement, XZECC_TACHE_PIC );
                  if (vl_EqtOk) vl_ValRet = ei_config_eqt_pic ( pl_Machine );
                  break;

               default: 
                  vl_ValRet = XDC_NOK;
                  break;
            }
         
            /* Fermeture des fichiers de configuration */
            /* S'il s'agit de l'equipement concerne */
            if ( ( pg_TabConf[vl_indice].TypeEqt == va_NumEqt ) || ( va_NumEqt == XZECC_TYPE_EQT_ALL ) ||
                 ( (va_NumEqt == XDC_EQT_PMV) && (pg_TabConf[vl_indice].TypeEqt == XDC_EQT_PCT) ) )
            {               
               if ( vl_ValRet == XDC_OK )
                   ec_ouv_ferm_fichier_config ( pl_Machine, vl_indice, EINI_FERMETURE_FIC );
               else
                   ec_ouv_ferm_fichier_config ( pl_Machine, vl_indice, EINI_DETRUIRE_FIC );
            }
            
            if ( ((pg_TabConf[vl_indice].TypeEqt == va_NumEqt) && (va_NumEqt != XDC_EQT_PMV)) ||
            	 ((pg_TabConf[vl_indice].TypeEqt == XDC_EQT_PCT) && (va_NumEqt == XDC_EQT_PMV)) ) break;
         }
            
         /*A Lecture en base du nom de la machine supleante */
         XZAO136_Lire_Suppleant ( EINI_NOM_BASE, pl_Machine->NomMachine, vl_NomMachineSup );
         /*strcpy ( pl_Machine->NomMachineSup, strtok(vl_NomMachineSup, " ") );*/
	 if ( vl_NomMachineSup!=(char *)NULL)
         	strcpy ( pl_Machine->NomMachineSup, vl_NomMachineSup );
	 else
	 	strcpy ( pl_Machine->NomMachineSup, "");
      }
   }
   
   /* Si l'equipement concerne est un PMV */
   if ( ( va_NumEqt == XZECC_TYPE_EQT_PMV ) || ( va_NumEqt == XZECC_TYPE_EQT_ALL ) )
   {
       /* Appel de la fonction ei_config_type_pmv du module MPMV */
       vl_ValRet = ei_config_type_pmv ( pl_Machine );
       /* Appel de la fonction ei_config_type_picto du module MPMV */
       vl_ValRet = ei_config_type_picto ( pl_Machine );
   }
   
   /* Si l'equipement concerne est une NAV */
   if ( ( va_NumEqt == XZECC_TYPE_EQT_NAV ) || ( va_NumEqt == XZECC_TYPE_EQT_ALL ) )
   {
       /* Appel de la fonction ei_config_type_nav du module MNAV */
       vl_ValRet = ei_config_type_nav ( pl_Machine );
   }

   /* Recherche de la machine concerne */
   for ( pl_Machine = pg_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant )
   {
      /* S'il s'agit de la machine concernee */
      if ( ((pl_Machine->TypeMachine == XZSCC_TYPEM_SD) || (pl_Machine->TypeMachine == XZSCC_TYPEM_SC) ||
	    (pl_Machine->TypeMachine == XZSCC_TYPEM_SP) || (pl_Machine->TypeMachine == XZSCC_TYPEM_LT)) &&
           ( !strcmp (pl_Machine->NomMachine, pa_NomMachine) ||
             !strcmp (pa_NomMachine, XZECC_TOUTES_MACHINES)) ) 
      {
         if ( strlen(pl_Machine->NomMachineSup) > 0 )
         {
             strcpy (vl_NomMachineSup, "");
             for ( pl_MachineSur = pg_ListeMachine; pl_MachineSur != NULL; pl_MachineSur = pl_MachineSur->Suivant )
             {
/*
Fix SAE-295
		if ( !strcmp (pl_MachineSur->NomMachineSup, pl_Machine->NomMachine) )
*/
                if ( !strncmp (pl_MachineSur->NomMachineSup, pl_Machine->NomMachine, strlen(pl_Machine->NomMachine)) )
                {
                   strcpy (vl_NomMachineSup, pl_MachineSur->NomMachine);
                   break;
                }
             }

             XZST_03EcritureTrace( XZSTC_DEBUG1,"La machine suppleante de %s est %s ",
                  				 pl_Machine->NomMachineSup,
                  				 pl_Machine->NomMachine );
             /* Creation du nouveau fichier de XDF_Machine_Sup */
             /* Construction du nom du fichier presence equipement */
             sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_DIFFUSION, XDF_Machine_Sup, pl_Machine->NomMachine );
      
             /* Ouverture du fichier presence equipement apres avoir detruit l'ancien*/
             XZSS_06DetruireFichier ( pl_PathEtFic );
             if ( (vl_Fd = open ( pl_PathEtFic, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) >= 0 )
             {
                sprintf ( pl_TexteConf, 	"#Nom de la machine suppleante \n%s\t%s\t", 
                				pl_Machine->NomMachineSup,
                				vl_NomMachineSup);
                write ( vl_Fd, pl_TexteConf, strlen (pl_TexteConf) );
             }
             /* Fermeture du fichier XDF_Machine_Sup */
             close ( vl_Fd );
          }
      }
   }

}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_liste_machine (	XDY_NomMachine	pa_NomMachine,
				int		va_NumOrdre,
				void * 		pa_Arg )

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine
*   int			va_NumOrdre
*   void * 		pa_Arg
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Reservation memoire
*   Insertion de la nouvelle machine 
*
------------------------------------------------------*/
{
   EINI_MACHINE 	*pl_NewMachine, *pl_Machine, *pl_ExMachine;
   int			vl_existe = XDC_NOK,
   			vl_Index  = 0;
		
   for ( pl_Machine = pl_ExMachine = pg_ListeMachine;
         pl_Machine != NULL;
         pl_ExMachine = pl_Machine, pl_Machine = pl_Machine->Suivant )
   {
       if ( strcmp (pa_NomMachine, pl_Machine->NomMachine) < 0 )
          break;
       if ( !strcmp (pa_NomMachine, pl_Machine->NomMachine) )
       {
          vl_existe = XDC_OK;
          break;
       }
   }
   if ( vl_existe != XDC_OK )
   {
      if ( ( pl_NewMachine = (EINI_MACHINE *) malloc ( sizeof(EINI_MACHINE) ) ) == NULL)
      {
         /* Ecriture trace */ 
         XZST_03EcritureTrace ( XZSTC_WARNING, " Allocation memoire impossible");
      }
      else
      {
      	 memset ( pl_NewMachine, 0, sizeof(EINI_MACHINE) );
         strcpy ( pl_NewMachine->NomMachine, pa_NomMachine );
         if ( vl_existe != XDC_OK )
         {
            if ( pl_Machine == pg_ListeMachine )
            {
                pl_NewMachine->Suivant    = pg_ListeMachine;
                pg_ListeMachine           = pl_NewMachine;
            }
            else
            {
               pl_NewMachine->Suivant = pl_ExMachine->Suivant;
               pl_ExMachine->Suivant  = pl_NewMachine;
            }
         }
      }
   }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Localisation des donnees d'un type d'equipement
*  dans la configuration.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_rang_eqt (	int		va_TypeEqt )

/*
* ARGUMENTS EN ENTREE :
*   int			va_TypeEqt
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/
{
int 	vl_indice = 0;

         /* Recherche de l'equipement concerne */
         for ( vl_indice = 0; vl_indice < pg_SizeTabConf; vl_indice++)
         {
            /* S'il s'agit de l'equipement concerne */
            if ( pg_TabConf[vl_indice].TypeEqt == va_TypeEqt ) 
            {
               return ( pg_TabConf[vl_indice].RangEqt ) ;
            }
         }
         return ( pg_SizeTabConf - 1 );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction creation du fichier des profils utilisateurs maintenance
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_creation_fich_utilisateurs ( )

/*
* ARGUMENTS EN ENTREE :
*   int			va_TypeEqt
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/
{
	int 	vl_ValRet = XDC_NOK;
	char 	pl_PathEtFic [XDC_PATH_ABS_SIZE];

	/* Construction du nom du fichier Utilisateurs */
	sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_DIFFUSION, XDF_Config_Utilisateurs );

	/* Ouverture du fichier Utilisateurs apres avoir detruit l'ancien */
	XZSS_06DetruireFichier ( pl_PathEtFic );
	if ( (vg_FdConf[0] = open ( pl_PathEtFic, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) >= 0 )
	{
	   /* Mise a jour liste des profils utilisateurs pour les LT */
	   vl_ValRet = ei_config_liste_profil ( );
	   close ( vg_FdConf[0] );
	}
	else
	{
	    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_PathEtFic );
	}
	return ( vl_ValRet );
}
