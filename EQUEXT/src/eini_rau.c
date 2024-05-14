/*E*/
/* Fichier : $Id: eini_rau.c,v 1.13 2019/05/21 11:03:18 pc2dpdy Exp $        Release : $Revision: 1.13 $        Date : $Date: 2019/05/21 11:03:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_rau.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache RAU >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	26 Sep 1994	: Creation
* Mismer.D	version 1.2	14 Dec 1994	:
* Fontaine.C	version 1.3	19 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.4	23 Oct 1996	: Modif pour Alerte PAU HS (DEM/1282)
* Mismer.D	version 1.5	18 Dec 1996	: Correction pour Alerte PAU HS (DEM/1282)
* Mismer.D	version 1.6	15 Jan 1998	: Ajout PC niveau 2 (DEM/1539)
* Cluchague     version 1.8	08 Mar 2002	: bidouille pour pc niveau 2 / 22/06/05 : ajout pour PC2LT aussi DEM213 
* XXXXX		version 1.9	04 Fev 2008	: Gestion machine SPCI et son maitre
* Niepceron	version 1.10	20 Mai 2008	: RAU au CI suppression de la bidouille pour pc niveau 2 DEM716
* JPL		15/02/12 : Migration architecture HP ia64 : format champs de 'printf'  1.11
* JMG		07/11/18 : ajout vitesse 1.12
* JMG		07/05/19 : GAPLA 1.13 DEM1334
* GGY		07/09/23 : Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_msok.h"

#include "eini_cfg.h"

/* definitions de constantes */

#define	CM_NB_PAU_RECOUVREMENT	6

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_rau.c,v 1.13 2019/05/21 11:03:18 pc2dpdy Exp $      : eini_rau" ;

XZEXT_CONFIG_PAU *	        pg_DebListePAU		= NULL;
static XDY_NomMachine	vm_MachinePAU = "";

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_RAU (	XZAOT_PAU 	va_PAU,
			int 		va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config des PAU
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumPAU\tCodePAU\tCodeAntenne\tType\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_RAU );
   }
   sprintf ( pl_TexteConf, 	"P %d\t%s\t%s\t%d\n\0",
   				va_PAU.Numero,
   				va_PAU.CodePAU,
   				va_PAU.CodeAntenne,
				va_PAU.Type);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_RAU %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_RAU );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_RAU" );
   return ( vl_ValRet );
}  


/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_eqt_rau ( 	EINI_MACHINE * 	pa_Machine )

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
	XZAOT_PIRAU	vl_PIRAU;
	XDY_Texte	pl_TexteConf;
	int		vl_ValRet = XDC_OK;
	XDY_Entier	vl_NbEqt = 0;

   	/* Lecture en base de donnees des paramatres de configuration */
   	strcpy ( pl_TexteConf,"                                                                             \n");
   	write ( vg_FdConf[0], pl_TexteConf, strlen(pl_TexteConf) );
   	XZST_03EcritureTrace ( XZSTC_INTERFACE, 
   				" Appel XZAO117_Lire_SystemeRAU avec EINI_NOM_BASE, %s, ei_config_RAU",
   				 pa_Machine->NomMachine );
   	vl_ValRet = XZAO117_Lire_SystemeRAU ( EINI_NOM_BASE,
   						 pa_Machine->NomMachine,
   						 ei_config_RAU,
   						 &vl_PIRAU,
   						 &vl_NbEqt );
   	if ( ( vl_ValRet == XDC_OK ) && ( /*vl_PIRAU.Numero*/vl_NbEqt != 0 ) )
   	{
   	    sprintf ( pl_TexteConf, "R %d\t%s\t%d\t%d\t%s\t%s\t%d\t%s\t%d\t\n#\0",
   	                		vl_PIRAU.Numero,
   	                		vl_PIRAU.ServeurTerminaux,
   	                		vl_PIRAU.LiaisonMaitre,
   	                		vl_PIRAU.LiaisonEsclave,
   	                		vl_PIRAU.Rgs,
							vl_PIRAU.AdresseIpEsclave,
							vl_PIRAU.PortIpEsclave,
							vl_PIRAU.AdresseIpMaitre,
							vl_PIRAU.PortIpMaitre);
   	    /* Ecriture dans le fichier de config */
   	    lseek ( vg_FdConf[0], 0, SEEK_SET);
   	    write ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf) );
   	    /* Memorisation existance RAU sur la machine et config ligne serie */
   	    strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RAU)].Equipement, XZECC_TACHE_RAU );
   	    pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RAU)].Present = XZEXC_EQT_PRESENT;

		if (!strcmp(vl_PIRAU.AdresseIpEsclave,XZECC_IP_VIDE))
		{
			ec_mise_a_jour_config_ligne (	vl_PIRAU.LiaisonMaitre,
											vl_PIRAU.ServeurTerminaux,
											XZECC_PROT_LCR,
											XZECC_SENS_MAITRE,
											XZECC_TYPE_EQT_RAU,
											vl_PIRAU.Numero,0 );
/*******************************************************************************************/
/* 	 bidouille pour les pc de niveau 2 et PC2LT*/
/*		if (( pa_Machine ->TypeMachine == XZSCC_TYPEM_PC2 ) || ( pa_Machine ->TypeMachine == XZSCC_TYPEM_PC2LT ))
		ec_mise_a_jour_config_ligne ( 	vl_PIRAU.LiaisonMaitre - 1,
								vl_PIRAU.ServeurTerminaux,
								XZECC_PROT_LCR,
								XZECC_SENS_MAITRE,
								XZECC_TYPE_EQT_RAU,
								vl_PIRAU.Numero );*/
/* 	 fin de la bidouille                */
/*******************************************************************************************/

			ec_mise_a_jour_config_ligne ( 	vl_PIRAU.LiaisonEsclave,
											vl_PIRAU.ServeurTerminaux,
											XZECC_PROT_LCR,
											XZECC_SENS_ESCLAVE,
											XZECC_TYPE_EQT_RAU,
											vl_PIRAU.Numero,0 );
		}
		else
		{
			ec_mise_a_jour_config_ligne (	vl_PIRAU.PortIpMaitre,
     										vl_PIRAU.AdresseIpMaitre,
											XZECC_PROT_IP,
											XZECC_SENS_MAITRE,
											XZECC_TYPE_EQT_RAU,
											vl_PIRAU.Numero,0 );
			
			ec_mise_a_jour_config_ligne (	vl_PIRAU.PortIpEsclave,
											vl_PIRAU.AdresseIpEsclave,
											XZECC_PROT_IP,
											XZECC_SENS_ESCLAVE,
											XZECC_TYPE_EQT_RAU,
											vl_PIRAU.Numero,0 );
		}
   	    return ( XDC_OK );
   	}
   	else
   	{
   	    /* Memorisation absence RAU sur la machine */
   	    strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RAU)].Equipement, XZECC_TACHE_RAU );
   	    pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RAU)].Present = XZEXC_EQT_ABSENT;
   	    return ( XDC_NOK );
   	}
                   
}

    
    
                  
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  Ajoute un PAU  pour fichier liste PAU pour alerte PAU HS
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_ajouter_info_PAU ( XZAOT_EqtSyn va_Eqt )

/*
* ARGUMENTS EN ENTREE :
*   va_Eqt : Info PAU a ajouter 
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   rien 
*
* FONCTION 
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
    XZEXT_CONFIG_PAU	  *pl_NewPAU;
    XDY_Nom		  vl_NomPR   = "",
    			  vl_TmpPR   = "";
    XDY_NomAuto		  vl_NomAuto = "";

   
    
    XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN : ec_ajouter_info_PAU Num:%d Nom:%s PR:%ld Sens:%d Autoroute:%d",
    				va_Eqt.NumeroEqt,
    				va_Eqt.NomEqt,
    				va_Eqt.PR,
    				va_Eqt.Sens ,
    				va_Eqt.NumeroAutoroute );

    strcpy ( vl_NomAuto, (va_Eqt.NumeroAutoroute < EINIC_NB_AUTOROUTE) ? vg_NomAutoroute[va_Eqt.NumeroAutoroute] : "???" );
    if ( (va_Eqt.PR > 0) && (va_Eqt.Sens > 0) )
    {
       sprintf ( vl_TmpPR, "%ld", (XDY_LongEntier) va_Eqt.PR );
       switch ( strlen(vl_TmpPR) )
       {
          case 4 :
             sprintf ( vl_NomPR, "%c.%c%c%c\0", vl_TmpPR[0],vl_TmpPR[1],vl_TmpPR[2],vl_TmpPR[3]);
             break;
          case 5 :
             sprintf ( vl_NomPR, "%c%c.%c%c%c\0", vl_TmpPR[0],vl_TmpPR[1],vl_TmpPR[2],vl_TmpPR[3],vl_TmpPR[4]);
             break;
          default :
             sprintf ( vl_NomPR, "%c%c%c.%c%c%c\0", vl_TmpPR[0],vl_TmpPR[1],vl_TmpPR[2],vl_TmpPR[3],vl_TmpPR[4],vl_TmpPR[5]);
             break;
       }
    }
    
   /*A Allocation memoire pour la nouvelle structure */
   if ( ( pl_NewPAU = (XZEXT_CONFIG_PAU *) malloc ( sizeof(XZEXT_CONFIG_PAU) ) ) == NULL)
   {
      /* Ecriture trace */ 
      XZST_03EcritureTrace ( XZSTC_WARNING, " ec_ajouter_info_PAU OUT: Allocation memoire impossible");
      return XDC_NOK;
   }

   /*A Mise a jour de la structure a inserer */ 
   strcpy ( pl_NewPAU->NomEqt, strtok(va_Eqt.NomEqt, " ") );
   sprintf ( pl_NewPAU->LibelleEqt, "%s %s-%d-%s\0", XDC_LIB_PAU, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
   pl_NewPAU->NumeroEqt 	= va_Eqt.NumeroEqt;
   pl_NewPAU->PR 		= va_Eqt.PR;
   pl_NewPAU->Sens 		= va_Eqt.Sens;
   pl_NewPAU->NumeroAutoroute 	= va_Eqt.NumeroAutoroute;
   pl_NewPAU->Maitre[0]	 	= '\0';
   pl_NewPAU->MaitreCI[0] 	= '\0';
   
   /*A Ajout de la structure en tete de liste */
   pl_NewPAU->Suivant = pg_DebListePAU;
   pg_DebListePAU     = pl_NewPAU;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :ec_ajouter_info_PAU");
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_maj_liste_PAU (	XZAOT_PAU 	va_PAU,
				int 		va_NumAppel )

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
*   Mise a jour du nom du maitre dams la liste des PAU
*
------------------------------------------------------*/
{
int			vl_ValRet = XDC_OK;
XZEXT_CONFIG_PAU	*pl_PAU;
   
		
   /*A Recherche du PAU dans la liste */
   for ( pl_PAU = pg_DebListePAU; pl_PAU != NULL; pl_PAU = pl_PAU->Suivant )
      if ( pl_PAU->NumeroEqt == va_PAU.Numero )
      {
	 if (!strcmp(vm_MachinePAU,"SPCI"))
	   strcpy(pl_PAU->MaitreCI, vm_MachinePAU);
	 else
           strcpy ( pl_PAU->Maitre, vm_MachinePAU );
         XZST_03EcritureTrace ( XZSTC_DEBUG1, " ei_maj_liste_PAU n�:%d maitre:%s", va_PAU.Numero, vm_MachinePAU );
         XZST_03EcritureTrace ( XZSTC_WARNING, " ei_maj_liste_PAU n�:%d maitre:%s", va_PAU.Numero, vm_MachinePAU );
         break;
      }
   return ( XDC_OK );
}  



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_recherche_maitre_PAU ( 	XDY_NomMachine pa_Machine )

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
*   Ecrire le parametre district dans la liste des PAU
*
------------------------------------------------------*/
{
XZAOT_PIRAU	vl_PIRAU;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt = 0;

   /* Lecture en base de donnees des paramatres de configuration */
   XZST_03EcritureTrace ( XZSTC_INTERFACE, 
   			" Appel XZAO117_Lire_SystemeRAU dans ei_recherche_maitre_PAU pour <%s>", pa_Machine );
   strcpy ( vm_MachinePAU, pa_Machine );
   vl_ValRet = XZAO117_Lire_SystemeRAU ( EINI_NOM_BASE,
   					 pa_Machine,
   					 ei_maj_liste_PAU,
   					 &vl_PIRAU,
   					 &vl_NbEqt );
   return ( XDC_OK );
                   
}
   
   
   
                  
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  Ajoute un PAU  pour fichier liste PAU pour alerte PAU HS
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_creer_fichier_info_PAU ( XDY_NomMachine va_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*   va_Eqt : Info PAU a ajouter 
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   rien 
*
* FONCTION 
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
    XZEXT_CONFIG_PAU	  *pl_PAU, *pl_AncPAU, * pl_TabPAU[CM_NB_PAU_RECOUVREMENT];
    int			  vl_Index = 0;
    int			  vl_FlagEcriture = XDC_FAUX;
    char 		  pl_NomFichier[XDC_PATH_ABS_SIZE];
    FILE		  *vl_Fd = NULL;
    XDY_NomMachine	  pl_NomMachine;
    char		  vl_Maitre[20];
    char		  vl_AncMaitre[20];

XZST_03EcritureTrace( XZSTC_WARNING,"ec_creer_fichier_info_PAU pour %s",va_NomMachine );
   /* Suppression de l'ancien fichier */
   sprintf ( pl_NomFichier, "%s/%s_%s", XZEXC_PATH_DIFFUSION, XDF_Config_PAU, va_NomMachine );
   if ( ( vl_Fd = fopen ( pl_NomFichier, "w" ) ) == NULL )
   {
       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
       return ( XDC_NOK );
   }
   fprintf ( vl_Fd, "# Nom du PAU\tNumero\tMaitre\tNom Base\tAutoroute\tPR\tSens\n" );
    
   for ( vl_Index = 0; vl_Index < CM_NB_PAU_RECOUVREMENT; vl_Index++ )
      pl_TabPAU[vl_Index] = NULL;
      
   /*A Recherche des PAU de la machine dans la liste */
   for ( pl_PAU = pl_AncPAU = pg_DebListePAU; pl_PAU != NULL; pl_AncPAU = pl_PAU, pl_PAU = pl_PAU->Suivant )
   {
      
      /* Si le maitre du PAU est different de la machine courante */
      if (!strcmp(va_NomMachine,"SPCI")) {
        strcpy(vl_Maitre,pl_PAU->MaitreCI);
        strcpy(vl_AncMaitre,pl_AncPAU->MaitreCI);
      }
      else {
        strcpy(vl_Maitre,pl_PAU->Maitre);
        strcpy(vl_AncMaitre,pl_AncPAU->Maitre);
      }

      if ( strcmp(vl_Maitre, va_NomMachine) &&  !strcmp(vl_AncMaitre, va_NomMachine) )
      {
         pl_TabPAU[0] = pl_PAU;
         for ( vl_Index = CM_NB_PAU_RECOUVREMENT - 1; vl_Index >= 0; vl_Index-- )
            if ( pl_TabPAU[0] != NULL )
               if ( (pl_AncPAU->NumeroAutoroute == pl_TabPAU[0]->NumeroAutoroute) &&
                    (pl_AncPAU->Sens == pl_TabPAU[0]->Sens) )
               {
                  vl_FlagEcriture = XDC_VRAI;
    	          fprintf ( vl_Fd, " %s,\t%d,\t%s,\t%s,\t%d,\t%ld,\t%d,\n",
    					pl_TabPAU[0]->LibelleEqt,
    					pl_TabPAU[0]->NumeroEqt,
    					pl_TabPAU[0]->Maitre,
    					pl_TabPAU[0]->NomEqt,
    					pl_TabPAU[0]->NumeroAutoroute,
    					(XDY_LongEntier) pl_TabPAU[0]->PR,
    					pl_TabPAU[0]->Sens );
                  pl_TabPAU[0] = pl_TabPAU[0]->Suivant;
               }
         pl_TabPAU[0] = NULL;
      }
      
      /* Si changement d'autoroute ou de sens alors RAZ des memo PAU */
      if ( (pl_AncPAU->NumeroAutoroute != pl_PAU->NumeroAutoroute) ||
           (pl_AncPAU->Sens != pl_PAU->Sens) )
      {
          for ( vl_Index = 0; vl_Index < CM_NB_PAU_RECOUVREMENT; vl_Index++ )
             pl_TabPAU[vl_Index] = NULL;
      }
      else
      {
          if ( strcmp(vl_Maitre, va_NomMachine) )
          {
             for ( vl_Index = CM_NB_PAU_RECOUVREMENT - 1; vl_Index > 0; vl_Index-- )
                pl_TabPAU[vl_Index] = pl_TabPAU[vl_Index-1];
             pl_TabPAU[0] = pl_PAU;
          }
      }
      
      /* Si le maitre du PAU est la machine courante */
      if ( !strcmp(vl_Maitre, va_NomMachine) )
      {
         if ( strcmp(vl_AncMaitre, pl_PAU->Maitre) )
         {
            for ( vl_Index = CM_NB_PAU_RECOUVREMENT -1 ; vl_Index >= 0; vl_Index-- )
               if ( pl_TabPAU[vl_Index] != NULL )
               {
                  vl_FlagEcriture = XDC_VRAI;
    	          fprintf ( vl_Fd, " %s,\t%d,\t%s,\t%s,\t%d,\t%ld,\t%d,\n",
    					pl_TabPAU[vl_Index]->LibelleEqt,
    					pl_TabPAU[vl_Index]->NumeroEqt,
    					pl_TabPAU[vl_Index]->Maitre,
    					pl_TabPAU[vl_Index]->NomEqt,
    					pl_TabPAU[vl_Index]->NumeroAutoroute,
    					(XDY_LongEntier) pl_TabPAU[vl_Index]->PR,
    					pl_TabPAU[vl_Index]->Sens );
                  pl_TabPAU[vl_Index] = NULL;
               }
         }
         vl_FlagEcriture = XDC_VRAI;
    	 fprintf ( vl_Fd, " %s,\t%d,\t%s,\t%s,\t%d,\t%ld,\t%d,\n",
    					pl_PAU->LibelleEqt,
    					pl_PAU->NumeroEqt,
    					pl_PAU->Maitre,
    					pl_PAU->NomEqt,
    					pl_PAU->NumeroAutoroute,
    					(XDY_LongEntier) pl_PAU->PR,
    					pl_PAU->Sens );
      }
   }

   fclose ( vl_Fd );
XZST_03EcritureTrace( XZSTC_WARNING,"ec_creer_fichier_info_PAU FlagEcriture=%d",vl_FlagEcriture );

   if ( vl_FlagEcriture )
   { 
      if ( ex_presence_machine(va_NomMachine) ==  XDC_OK )
      {
          sprintf ( pl_NomFichier, "%s_%s", XDF_Config_PAU, va_NomMachine );
          if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
          {
             XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
          }
          else
          {
             if ( XZSS_07TransfertFichier ( pl_NomMachine,
                  			    pl_NomFichier,
                  			    XZEXC_PATH_DIFF_DEST,
               		   	   	    va_NomMachine,
               				    pl_NomFichier,
               				    XZEXC_PATH_DIFF_DEST ) != XDC_OK )
             {
                 XZST_03EcritureTrace( XZSTC_WARNING, "Transfert fichier %s de %s vers %s impossible",
               					       pl_NomFichier,
               					       pl_NomMachine,
               					       va_NomMachine );
             } 
             else
             {
                 XZST_03EcritureTrace( XZSTC_INFO, "Transfert fichier %s de %s vers %s OK",
               					    pl_NomFichier,
               					    pl_NomMachine,
               					    va_NomMachine );
             }
          }
      }
   }
   else
      XZSS_06DetruireFichier ( pl_NomFichier );
         
         
  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :ec_creer_fichier_info_PAU");
   return ( XDC_OK );
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_alerte_PAU_HS ( 	EINI_MACHINE * 	pa_ListeMachine, XDY_NomMachine pa_NomMachine )

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
   XZEXT_CONFIG_PAU	*pl_PAU, *pl_AncPAU;
   EINI_MACHINE 	*pl_Machine = NULL;
   XDY_Entier		vl_result = 0;
   int			vl_cr = XDC_OK;

   XZST_03EcritureTrace( XZSTC_WARNING,  "Debut config pour Alerte PAU HS" );


   /* Detruire la liste des PAU du SAE */
   for ( pl_PAU = pl_AncPAU = pg_DebListePAU;
         pl_PAU != NULL;
         pl_AncPAU = pl_PAU  )
   {
      /*A Liberation memoire */
      pl_PAU = pl_PAU->Suivant;
      free ( pl_AncPAU );
   }
   pg_DebListePAU = NULL;
   
   /* Creation de la liste des PAU du SAE */
   if ( XZAO125_Liste_Eqt_Synoptique( EINI_NOM_BASE, NULL, NULL, XDC_EQT_PAU, ec_ajouter_info_PAU, &vl_result ) == XDC_NOK )
   {
       XZST_03EcritureTrace( XZSTC_WARNING,  "Probleme lors de XZAO125_Liste_Eqt_Synoptique pour les PAU" );
       vl_cr = XDC_NOK;
   }
   
   /* Recherche de la machine maitre de chaque PAU */
   for ( pl_Machine = pa_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant )
   {
      if ( (pl_Machine->TypeMachine == XZSCC_TYPEM_SP) || (pl_Machine->TypeMachine == XZSCC_TYPEM_PCS) ||
      	   (pl_Machine->TypeMachine == XZSCC_TYPEM_PC2) || (pl_Machine->TypeMachine == XZSCC_TYPEM_PC2LT) ) 
      {
          ei_recherche_maitre_PAU ( pl_Machine->NomMachine );
      }
   }
   
   /* Creation des fichiers pour alerte PAU HS pour chaque machine maitre */
   for ( pl_Machine = pa_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant )
   {
      if ( (pl_Machine->TypeMachine == XZSCC_TYPEM_SP) || (pl_Machine->TypeMachine == XZSCC_TYPEM_PCS) ||
      	   (pl_Machine->TypeMachine == XZSCC_TYPEM_PC2) || (pl_Machine->TypeMachine == XZSCC_TYPEM_PC2LT)) 
      {
          ec_creer_fichier_info_PAU ( pl_Machine->NomMachine );
      }
   }
   
   XZST_03EcritureTrace( XZSTC_WARNING,  "Fin config pour Alerte PAU HS" );

   return (vl_cr);
}
