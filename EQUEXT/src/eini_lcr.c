/*E*/
/* Fichier :  $Id: eini_lcr.c,v 1.23 2018/10/22 15:19:26 pc2dpdy Exp $      Release : $Revision: 1.23 $        Date : $Date: 2018/10/22 15:19:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_lcr.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config des equipements pour fenetre LCR>
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	13 Dec 1994	: Creation
* Mismer.D 	version 1.2 	21 Dec 1994 	:
* Mismer.D 	version 1.3 	23 Jan 1995 	:
* Mismer.D 	version 1.4 	25 Jan 1995 	:
* Mismer.D 	version 1.5 	09 Fev 1995 	:
* Mismer.D 	version 1.6 	10 Fev 1995 	:
* Mismer.D 	version 1.7 	13 Avr 1995 	:
* Fontaine.C 	version 1.8 	19 Mai 1995 	: Reprise entete, historique
* Mismer.D 	version 1.9 	30 Mai 1995 	: Modif nom equipement dans fichier LCR
* Mismer.D 	version 1.10 	20 Jun 1995 	: Modif parametre XZAO123
* Mismer.D 	version 1.11 	08 Aou 1995 	: Modif nom equipement GTC
* Mismer.D 	version 1.12 	12 Dec 1995 	: Modif mise a jour fichier LCR
* Mismer.D  	version 1.13	10 Sep 1996	: Ajout type machine (RADT_W) 
* Mismer.D	version 1.14	23 Oct 1996	: Modif pour Alerte PAU HS (DEM/1282)
* Mismer.D	version 1.15	07 Nov 1996	: Modif METEO (DEM/1232)
* Mismer.D	version 1.16	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.17	15 Jan 1998	: Modif PAL (DEM/1539)
* JMG		version 1.19	11/10/05	: ajout PMVA BAF BAD site gestion
* JMG   13/11/07        : ajout site de gestion DEM715
* VR	20/12/11		: Ajout PRV (DEM/1016)
* JPL	24/03/17	: Suppression de la generation de la configuration video (DEM 1189)  1.22
* JMG	04/10/18	: ajout CAP 1.23
* ABK 29/08/23 : Ajout nouvel equipement picto (DEM/483)
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_syn.h"

#include "eini_lcr.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_lcr.c,v 1.23 2018/10/22 15:19:26 pc2dpdy Exp $      : eini_lcr" ;

EINI_DONNEES_FEN_LCR *	        pg_DebListeRAU		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeQTX		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeTDP		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeVID		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeRDT		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeDAI		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListePMV		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListePICTO	= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeNAV		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListePAL		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeGTC		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListePMVA		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeBAF		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeBAD		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListePRV		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListePIC		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_DebListeCAP		= NULL;
EINI_DONNEES_FEN_LCR *	        pg_ListeTmp		= NULL;

EINI_MACHINE *			pm_ListeMachineLCR	= NULL;
EINI_MACHINE *			pm_MachineLCR		= NULL;

static XDY_Entier		vm_TypeEqtLcr = 0;

/* declaration de variables globales */
extern XDY_Mot		vg_SitePrincipal;

/* declaration de fonctions internes */

int   ei_config_alerte_PAU_HS ( EINI_MACHINE *, XDY_NomMachine );

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  Ajoute un type d'equipement pour fichier fenetre LCR
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_ajouter_eqt_lcr ( XZAOT_EqtSyn va_Eqt )

/*
* ARGUMENTS EN ENTREE :
*   va_Eqt : Descriptif de l'equipement a ajouter 
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
*   Positionnement d'un pointeur sur le debut de la liste du type d'equipement concernee
*   Recherche de l'emplacement pour l'insertion
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
    EINI_DONNEES_FEN_LCR 	*pl_NewEqtLCR, *pl_EqtLCR, *pl_ExEqtLCR;
    XDY_Nom			vl_NomEqt  = "";
    XDY_Nom			vl_NomPR   = "",
    				vl_TmpPR   = "";
    XDY_NomAuto			vl_NomAuto = "";

	strcpy ( vl_NomAuto, (va_Eqt.NumeroAutoroute < EINIC_NB_AUTOROUTE) ? vg_NomAutoroute[va_Eqt.NumeroAutoroute] : "???" );

    if ( (va_Eqt.PR > 0) && (va_Eqt.Sens > 0) )
    {
       sprintf ( vl_TmpPR, "%ld", (long) va_Eqt.PR );
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
    
    XZST_03EcritureTrace(   XZSTC_WARNING, "IN : ec_ajouter_eqt_lcr Num:%d Nom:%s PR:%ld Sens:%d Autoroute:%s",
    				va_Eqt.NumeroEqt,
    				va_Eqt.NomEqt,
    				va_Eqt.PR,
    				va_Eqt.Sens ,
    				vl_NomAuto );
  
   /* Selon le type d'equipement: positionner un pointeur sur le debut de la liste*/
   switch ( vm_TypeEqtLcr )
   {
      case XZECC_TYPE_EQT_RAU :
         sprintf ( vl_NomEqt, "%s %s\0", XDC_LIB_RAU, va_Eqt.NomEqt );
         break;
      case XZECC_TYPE_EQT_VID :
         sprintf ( vl_NomEqt, "%s %s\0", XDC_LIB_SYV, va_Eqt.NomEqt );
         break;
      case XZECC_TYPE_EQT_RDT :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_RAD, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XZECC_TYPE_EQT_DAI :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_DAI, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XZECC_TYPE_EQT_PMV :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_PMV, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
		 XZST_03EcritureTrace ( XZSTC_WARNING, "ec_ajouter_eqt_lcr PMV Nom:%s", va_Eqt.NomEqt);
         break;
      case XZECC_TYPE_EQT_PMVA :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_PMVA, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XZECC_TYPE_EQT_PICTO :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_PCT, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
		 XZST_03EcritureTrace ( XZSTC_WARNING, "ec_ajouter_eqt_lcr Picto Nom:%s", va_Eqt.NomEqt);
         break;
      case XZECC_TYPE_EQT_NAV :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_NAV, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XZECC_TYPE_EQT_BAF :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_BAF, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XZECC_TYPE_EQT_BAD :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_BAD, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
	  case XZECC_TYPE_EQT_PRV :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_PRV, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XZECC_TYPE_EQT_PIC :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_PIC, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XDC_EQT_CAP:
      	sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_CAP, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
	break;
      case XDC_EQT_FAU :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_FAU, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XDC_EQT_FAC :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_FAC, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XZECC_TYPE_EQT_PAL :
         sprintf ( vl_NomEqt, "%s %s-%d-%s\0", XDC_LIB_PAL, vl_NomAuto, va_Eqt.Sens, vl_NomPR );
         break;
      case XZECC_TYPE_EQT_GTC :
         sprintf ( vl_NomEqt, "%s %s\0", XDC_LIB_GTC, va_Eqt.NomEqt );
         break;
      default : return XDC_NOK;
   }
		
   /*A Recherche si l'equipement existe dans la liste */
   for ( pl_EqtLCR = pl_ExEqtLCR = pg_ListeTmp;
         pl_EqtLCR != NULL;
         pl_ExEqtLCR = pl_EqtLCR, pl_EqtLCR = pl_EqtLCR->Suivant )
   {
       if ( pl_EqtLCR->NumEqt == va_Eqt.NumeroEqt )
       {
          strcpy ( pl_EqtLCR->NomEqt, vl_NomEqt );
		  XZST_03EcritureTrace ( XZSTC_WARNING, "ec_ajouter_eqt_lcr eqt dans liste Nom:%s", va_Eqt.NomEqt);
          return XDC_OK;
       }
   }

   /*A Recherche de la position d'insertion dans la liste */
   for ( pl_EqtLCR = pl_ExEqtLCR = pg_ListeTmp;
         pl_EqtLCR != NULL;
         pl_ExEqtLCR = pl_EqtLCR, pl_EqtLCR = pl_EqtLCR->Suivant )
   {
       if ( (pl_EqtLCR->Sens >= va_Eqt.Sens) &&
            (pl_EqtLCR->NumeroAutoroute >= va_Eqt.NumeroAutoroute) &&
            (pl_EqtLCR->PR <= va_Eqt.PR) )
          break;
   }



   /*A Allocation memoire pour la nouvelle structure */
   if ( ( pl_NewEqtLCR = (EINI_DONNEES_FEN_LCR *) malloc ( sizeof(EINI_DONNEES_FEN_LCR) ) ) == NULL)
   {
      /* Ecriture trace */ 
      XZST_03EcritureTrace ( XZSTC_WARNING, "ec_ajouter_eqt_lcr OUT Nom:%s : Allocation memoire impossible", va_Eqt.NomEqt);
      return XDC_NOK;
   }

   /*A Mise a jour de la structure a inserer */
   memset ( pl_NewEqtLCR, 0, sizeof(EINI_DONNEES_FEN_LCR) );
   strcpy ( pl_NewEqtLCR->NomEqt, vl_NomEqt );
   pl_NewEqtLCR->NumEqt = va_Eqt.NumeroEqt;
   pl_NewEqtLCR->Sens = va_Eqt.Sens;
   pl_NewEqtLCR->NumeroAutoroute = va_Eqt.NumeroAutoroute;
   pl_NewEqtLCR->PR = va_Eqt.PR;
   pl_NewEqtLCR->SiteGestion = va_Eqt.SiteGestion;
   pl_NewEqtLCR->TypeEqt = vm_TypeEqtLcr;
   /*A Ajout de la structure dans la liste */
   if ( pl_EqtLCR == pg_ListeTmp )
   {
      pl_NewEqtLCR->Suivant = pg_ListeTmp;
      pg_ListeTmp           = pl_NewEqtLCR;
   }
   else
   {
      pl_NewEqtLCR->Suivant = pl_ExEqtLCR->Suivant;
      pl_ExEqtLCR->Suivant  = pl_NewEqtLCR;
   }

   XZST_03EcritureTrace(   XZSTC_WARNING, "OUT :ec_ajouter_eqt_lcr Nom:%s ", va_Eqt.NomEqt);
   return XDC_OK;
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  Ajoute un nom d'autoroute pour creation nom equipement fichier LCR
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_ajouter_autoroute ( 	XDY_Autoroute 	va_NumAuto,
				XDY_NomAuto 	va_NomAuto )

/*
* ARGUMENTS EN ENTREE :
*  XDY_Autoroute 	va_NumAuto	: Numero de l'autoroute
*  XDY_NomAuto 		va_NomAuto	: Nom de l'autoroute
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
*
------------------------------------------------------*/
{
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN : ec_ajouter_autoroute Num:%d Nom:%s",
    				va_NumAuto,
    				va_NomAuto );
  
   if ( va_NumAuto < EINIC_NB_AUTOROUTE )
      strcpy ( vg_NomAutoroute[va_NumAuto], strtok(va_NomAuto, " ") );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :ec_ajouter_autoroute");
   return XDC_OK;
}




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  Ajoute un type d'equipement pour fichier fenetre LCR
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_ajouter_machine_lcr ( XZAOT_EqtSyn va_Eqt )

/*
* ARGUMENTS EN ENTREE :
*   va_Eqt : Descriptif de l'equipement a ajouter 
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
*   Positionnement d'un pointeur sur le debut de la liste du type d'equipement concernee
*   Recherche de l'emplacement pour l'insertion
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
    EINI_DONNEES_FEN_LCR 	*pl_NewEqtLCR, *pl_EqtLCR, *pl_ExEqtLCR;
   
    XZST_03EcritureTrace(   XZSTC_FONCTION, "IN ec_ajouter_machine_lcr:  Num:%d Nom:%s",
    				va_Eqt.NumeroEqt,
    				va_Eqt.NomEqt );

   /*A Recherche si la machine est un serveur de protocole ou un poste operateur */
   if ( !strncmp ( va_Eqt.NomEqt, "PO", 2 ) || !strncmp ( va_Eqt.NomEqt, "SP", 2 ) || !strncmp ( va_Eqt.NomEqt, "SD", 2 ) || !strncmp ( va_Eqt.NomEqt, "ST", 2 ))
   {
      XZST_03EcritureTrace ( XZSTC_INFO, "Creation fichiers config LCR de %s", va_Eqt.NomEqt );
   }
   else
      return ( XDC_OK );
		
   /*A Recherche de la position d'insertion dans la liste */
   for ( pl_EqtLCR = pl_ExEqtLCR = pg_DebMacLCR;
         pl_EqtLCR != NULL;
         pl_ExEqtLCR = pl_EqtLCR, pl_EqtLCR = pl_EqtLCR->Suivant )
   {
       if ( va_Eqt.NumeroEqt < pl_EqtLCR->NumEqt )
          break;
       if ( pl_EqtLCR->NumEqt == va_Eqt.NumeroEqt )
          return XDC_OK;
   }

   /*A Allocation memoire pour la nouvelle structure */
   if ( ( pl_NewEqtLCR = (EINI_DONNEES_FEN_LCR *) malloc ( sizeof(EINI_DONNEES_FEN_LCR) ) ) == NULL)
   {
      /* Ecriture trace */ 
      XZST_03EcritureTrace ( XZSTC_WARNING, " Allocation memoire impossible");
      XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : ec_ajouter_machine_lcr");
      return XDC_NOK;
   }

   /*A Mise a jour de la structure a inserer */
   memset ( pl_NewEqtLCR, 0, sizeof(EINI_DONNEES_FEN_LCR) );
   strcpy ( pl_NewEqtLCR->NomEqt, strtok(va_Eqt.NomEqt, " ") );
   pl_NewEqtLCR->NumEqt = va_Eqt.NumeroEqt;
   
   /*A Ajout de la structure dans la liste */
   if ( pl_EqtLCR == pg_DebMacLCR )
   {
      pl_NewEqtLCR->Suivant = pg_DebMacLCR;
      pg_DebMacLCR          = pl_NewEqtLCR;
   }
   else
   {
      pl_NewEqtLCR->Suivant = pl_ExEqtLCR->Suivant;
      pl_ExEqtLCR->Suivant  = pl_NewEqtLCR;
   }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :ec_ajouter_machine_lcr");
   return XDC_OK;
}

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config_lcr (	int		va_TypeEqt )

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
*  Recuperation en base avec XZAO125 de la liste des equipement par type
*
------------------------------------------------------*/
{
    int						vl_indice = 0,
							vl_ValRet = XDC_OK;
    XDY_Entier				vl_result = 0;
    EINI_DONNEES_FEN_LCR 	*pl_EqtLCR, *pl_ExEqtLCR, *pl_Debut, *pl_NewEqtLCR;

	/*A Creation de liste des autoroute par appel de la primitive XZAO123 */
	if ( XZAO123_Liste_Autoroute( EINI_NOM_BASE, ec_ajouter_autoroute ) == XDC_NOK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING,  "Probleme lors de XZAO123_Liste_Autoroute pour liste autoroute");
	}
        
      /* Recherche de l'equipement concerne */
      for ( vl_indice = 0; vl_indice < pg_SizeTabConf; vl_indice++)
      {
         /* S'il s'agit de l'equipement concerne */
         if ( ( pg_TabConf[vl_indice].TypeEqt == va_TypeEqt ) || ( va_TypeEqt == XZECC_TYPE_EQT_ALL ) )
         {
             /* Selon le type d'equipement: positionner un pointeur sur le debut de la liste */
             vl_ValRet = XDC_OK;

             switch ( pg_TabConf[vl_indice].TypeEqt )
             {
                 case XZECC_TYPE_EQT_RAU :
                    pl_Debut = pg_DebListeRAU;
                    break;
                 case XZECC_TYPE_EQT_VID :
                    pl_Debut = pg_DebListeVID;
                    break;
                 case XZECC_TYPE_EQT_RDT :
                    pl_Debut = pg_DebListeRDT;
                    break;
                 case XZECC_TYPE_EQT_DAI :
                    pl_Debut = pg_DebListeDAI;
                    break;
                 case XZECC_TYPE_EQT_PMV :
                    pl_Debut = pg_DebListePMV;
                    break;
				 case XZECC_TYPE_EQT_PMVA :
					pl_Debut = pg_DebListePMVA;
					break;
                 case XZECC_TYPE_EQT_PICTO :
                    pl_Debut = pg_DebListePICTO;
                    break;
                 case XZECC_TYPE_EQT_NAV :
                    pl_Debut = pg_DebListeNAV;
                    break;
                 case XZECC_TYPE_EQT_BAF :
                    pl_Debut = pg_DebListeBAF;
                    break;
                 case XZECC_TYPE_EQT_BAD :
                    pl_Debut = pg_DebListeBAD;
                    break;
				 case XZECC_TYPE_EQT_PRV :
					pl_Debut = pg_DebListePRV;
					break;
            case XZECC_TYPE_EQT_PIC :
					pl_Debut = pg_DebListePIC;
					break;
				case XZECC_TYPE_EQT_CAP:
					pl_Debut = pg_DebListeCAP;
					break;
                 case XZECC_TYPE_EQT_PAL :
                    pl_Debut = pg_DebListePAL;
                    break;
                 case XZECC_TYPE_EQT_GTC :
                    pl_Debut = pg_DebListeGTC;
                    break;
                 default : vl_ValRet = XDC_NOK;
             }
         
             if ( vl_ValRet == XDC_OK )
             {
                vm_TypeEqtLcr = pg_TabConf[vl_indice].TypeEqt;
                pg_ListeTmp = NULL;
                /*A Creation de la nouvelle liste par appel de la primitive XZAO125 */
                if ( XZAO125_Liste_Eqt_Synoptique( EINI_NOM_BASE, NULL, NULL, vm_TypeEqtLcr, ec_ajouter_eqt_lcr ,&vl_result ) == XDC_NOK )
                {
                   XZST_03EcritureTrace(   XZSTC_WARNING,  "Probleme lors de XZAO125_Liste_Eqt_Synoptique avec type:%d ", vm_TypeEqtLcr);
                }
                if ( pg_TabConf[vl_indice].TypeEqt == XZECC_TYPE_EQT_NAV )
                {
                   vm_TypeEqtLcr = XDC_EQT_FAU;
                   if ( XZAO125_Liste_Eqt_Synoptique( EINI_NOM_BASE, NULL, NULL, vm_TypeEqtLcr, ec_ajouter_eqt_lcr ,&vl_result ) == XDC_NOK )
                   {
                      XZST_03EcritureTrace(   XZSTC_WARNING,  "Probleme lors de XZAO125_Liste_Eqt_Synoptique avec type:%d ", vm_TypeEqtLcr);
                   }
                   vm_TypeEqtLcr = XDC_EQT_FAC;
                   if ( XZAO125_Liste_Eqt_Synoptique( EINI_NOM_BASE, NULL, NULL, vm_TypeEqtLcr, ec_ajouter_eqt_lcr ,&vl_result ) == XDC_NOK )
                   {
                      XZST_03EcritureTrace(   XZSTC_WARNING,  "Probleme lors de XZAO125_Liste_Eqt_Synoptique avec type:%d ", vm_TypeEqtLcr);
                   }
                }
                if ( pl_Debut == NULL )
                {
                   /* Creation de liste */
                   pl_Debut = pg_ListeTmp;
                }
                else
                {
                   /*A pour chaque equipement  */
                   for ( pl_ExEqtLCR = pg_ListeTmp; pl_ExEqtLCR != NULL; pl_ExEqtLCR = pl_ExEqtLCR->Suivant  )
                   {
                      for ( pl_EqtLCR = pl_Debut; pl_EqtLCR != NULL; pl_EqtLCR = pl_EqtLCR->Suivant )
                      {
                         if ( pl_EqtLCR->NumEqt == pl_ExEqtLCR->NumEqt )
                         {
                            strcpy ( pl_EqtLCR->NomEqt, pl_ExEqtLCR->NomEqt );
                            break;
                         }
                         if ( pl_EqtLCR->Suivant == NULL )
                         {
                            /* Insertion */
                            /*A Allocation memoire pour la nouvelle structure */
                            if ( ( pl_NewEqtLCR = (EINI_DONNEES_FEN_LCR *) malloc ( sizeof(EINI_DONNEES_FEN_LCR) ) ) == NULL)
                            {
                               /* Ecriture trace */ 
                               XZST_03EcritureTrace ( XZSTC_WARNING, "ec_config_lcr : Allocation memoire impossible");
                               return;
                            }
                            memcpy ( pl_NewEqtLCR, pl_ExEqtLCR, sizeof(EINI_DONNEES_FEN_LCR) );
                            pl_EqtLCR->Suivant = pl_NewEqtLCR;
                            pl_NewEqtLCR->Suivant = NULL;
                            break;
                         }
                      }
                   }

                   /*A liberation de la liste temporaire  */
                   for ( pl_EqtLCR = pl_ExEqtLCR = pg_ListeTmp;
                         pl_EqtLCR != NULL;
                         pl_ExEqtLCR = pl_EqtLCR  )
                   {
                       /*A Liberation memoire */
                       pl_EqtLCR = pl_EqtLCR->Suivant;
                       free ( pl_ExEqtLCR );
                    }
                 }



				for ( pl_EqtLCR = pl_Debut; pl_EqtLCR != NULL; pl_EqtLCR = pl_EqtLCR->Suivant )
				{
					if ((vg_SitePrincipal != XDC_CI) && (strlen(pl_EqtLCR->NomLT) == 0))
					{
						ec_mise_a_jour_lcr ( pg_TabConf[vl_indice].TypeEqt, pl_EqtLCR->NumEqt );
					}
					XZST_03EcritureTrace ( XZSTC_WARNING, "ec_config_lcr, : EQT=%d NOM=%s LT=%s", pl_EqtLCR->NumEqt, pl_EqtLCR->NomEqt, pl_EqtLCR->NomLT );
				}

                 /* Selon le type d'equipement: positionner un pointeur sur le debut de la liste */
                 switch ( pg_TabConf[vl_indice].TypeEqt )
                 {
                     case XZECC_TYPE_EQT_RAU :
                        pg_DebListeRAU = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_VID :
                        pg_DebListeVID = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_RDT :
                        pg_DebListeRDT = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_DAI :
                        pg_DebListeDAI = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_PMV :
                        pg_DebListePMV = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_PMVA :
                        pg_DebListePMVA = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_PICTO :
                        pg_DebListePICTO = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_BAF :
                        pg_DebListeBAF = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_BAD :
                        pg_DebListeBAD = pl_Debut;
                        break;
					      case XZECC_TYPE_EQT_PRV :
                        pg_DebListePRV = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_PIC :
                        pg_DebListePIC = pl_Debut;
                        break;
		     case XZECC_TYPE_EQT_CAP:
		     	pg_DebListeCAP = pl_Debut;
			break;
                     case XZECC_TYPE_EQT_NAV :
                        pg_DebListeNAV = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_PAL :
                        pg_DebListePAL = pl_Debut;
                        break;
                     case XZECC_TYPE_EQT_GTC :
                        pg_DebListeGTC = pl_Debut;
                        break;
                 }
            }
         }
      }

   
      if ( (va_TypeEqt == XDC_EQT_GEN_LCR) || (va_TypeEqt == XZECC_TYPE_EQT_ALL) )
      {
         ei_config_Fentre_LCR_GEN_LCR ( XZECC_TOUTES_MACHINES );
      }
      if ( (va_TypeEqt == XDC_EQT_TDP) || (va_TypeEqt == XZECC_TYPE_EQT_ALL) )
      {
         ei_config_TDP_Fenetre_LCR ( XZECC_TOUTES_MACHINES );
      }
   
      /*A Creation de la nouvelle liste par appel de la primitive XZAO125 */
      if ( XZAO125_Liste_Eqt_Synoptique( EINI_NOM_BASE, NULL, NULL, XDC_EQT_MAC, ec_ajouter_machine_lcr ,&vl_result ) == XDC_NOK )
      {
          XZST_03EcritureTrace(   XZSTC_WARNING,  "Probleme lors de XZAO125_Liste_Eqt_Synoptique ");
      }
      
   
}




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int  ec_mise_a_jour_lcr ( XDY_Eqt	va_TypeEqt,
			   XDY_Eqt	va_NumEqt )

/*
* ARGUMENTS EN ENTREE :
*   
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
*   
*
------------------------------------------------------*/
{
    EINI_DONNEES_FEN_LCR 	*pl_EqtLCR, *pl_Debut;
    EINI_MACHINE *		pl_MachineLCR	= NULL;

    if ( pm_MachineLCR != NULL ) 
       pl_MachineLCR = pm_MachineLCR;
    else
       pl_MachineLCR = pg_MachineCrt;
      
   /* Selon le type d'equipement: positionner un pointeur sur le debut de la liste*/
   switch (  va_TypeEqt )
   {
      case XZECC_TYPE_EQT_RAU :
         pl_Debut = pg_DebListeRAU;
         break;
      case XZECC_TYPE_EQT_VID :
         pl_Debut = pg_DebListeVID;
         break;
      case XZECC_TYPE_EQT_RDT :
         pl_Debut = pg_DebListeRDT;
         break;
      case XZECC_TYPE_EQT_DAI :
         pl_Debut = pg_DebListeDAI;
         break;
      case XZECC_TYPE_EQT_PMV :
         pl_Debut = pg_DebListePMV;
         break;
      case XZECC_TYPE_EQT_PMVA :
         pl_Debut = pg_DebListePMVA;
         break;
      case XZECC_TYPE_EQT_BAF :
         pl_Debut = pg_DebListeBAF;
         break;
      case XZECC_TYPE_EQT_BAD :
         pl_Debut = pg_DebListeBAD;
         break;
	  case XZECC_TYPE_EQT_PRV :
         pl_Debut = pg_DebListePRV;
		 break;
     case XZECC_TYPE_EQT_PIC :
         pl_Debut = pg_DebListePIC;
		 break;
      case XZECC_TYPE_EQT_CAP:
      	pl_Debut = pg_DebListeCAP;
	break;
      case XZECC_TYPE_EQT_PICTO :
         pl_Debut = pg_DebListePICTO;
         break;
      case XZECC_TYPE_EQT_NAV :
         pl_Debut = pg_DebListeNAV;
         break;
      case XZECC_TYPE_EQT_PAL :
         pl_Debut = pg_DebListePAL;
         break;
      case XZECC_TYPE_EQT_GTC :
         pl_Debut = pg_DebListeGTC;
         break;
      default : return ( (pm_MachineLCR != NULL) ? XDC_NOK : XDC_OK );
   }
		
   /*A Recherche de l'equipement concerne */
   for ( pl_EqtLCR = pl_Debut;
         pl_EqtLCR != NULL;
         pl_EqtLCR = pl_EqtLCR->Suivant )
   {
          XZST_03EcritureTrace(   XZSTC_WARNING, "ec_mise_a_jour_lcr pl_EqtLCR->NumEqt=%d, va_NumEqt=%d.", pl_EqtLCR->NumEqt, va_NumEqt);

       /*A Si equipemnt trouve */
       if ( pl_EqtLCR->NumEqt == va_NumEqt )
       {
          /*A Mise a jour du nom du LT */
          strcpy ( pl_EqtLCR->NomLT,  pl_MachineLCR->NomMachine );
          return ( (pm_MachineLCR != NULL) ? XDC_NOK : XDC_OK );
          break;
       }
   }

   XZST_03EcritureTrace(   XZSTC_WARNING, "OUT :ec_mise_a_jour_lcr");
   return ( (pm_MachineLCR != NULL) ? XDC_NOK : XDC_OK );
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

void	ec_fichier_lcr ( )

/*
* ARGUMENTS EN ENTREE :
*   Aucun
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
   int				vl_indice = 0,
   				vl_FlagEcriture = 0,
				vl_ValRet = XDC_OK;
   EINI_DONNEES_FEN_LCR 	*pl_EqtLCR, *pl_Debut;
   char 			pl_NomFichier[XDC_PATH_ABS_SIZE];
   XDY_Fichier			vl_Fd = 0;
   XDY_Texte			pl_TexteConf;

         /* Recherche de l'equipement concerne */
         for ( vl_indice = 0; vl_indice < pg_SizeTabConf; vl_indice++)
         {
            if ( ( pg_TabConf[vl_indice].TypeFichier1 == EINI_TYPE_CONF ) &&
                 ( strlen (pg_TabConf[vl_indice].NomFichier1) > 0 ) )
            {
               /* Suppression de l'ancien fichier */
               sprintf ( pl_NomFichier, "%s/%s_LCR", 
               			XZEXC_PATH_DIFFUSION, 
               			pg_TabConf[vl_indice].NomFichier1 );
               /* Alors creation du nouveau fichier de config */
               XZSS_06DetruireFichier ( pl_NomFichier );
               vl_FlagEcriture = 0;
               if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
               {
                  XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
               }
               else
               {
                  vl_ValRet = XDC_OK;
                  /* Selon le type d'equipement: positionner un pointeur sur le debut de la liste*/
                  switch ( pg_TabConf[vl_indice].TypeEqt )
                  {
                     case XZECC_TYPE_EQT_RAU :
                        pl_Debut = pg_DebListeRAU;
                        break;
                     case XDC_EQT_GEN_LCR :
                        pl_Debut = pg_DebListeQTX;
                        break;
                     case XDC_EQT_TDP :
                        pl_Debut = pg_DebListeTDP;
                        break;
                     case XZECC_TYPE_EQT_VID :
                        pl_Debut = pg_DebListeVID;
                        break;
                     case XZECC_TYPE_EQT_RDT :
                        pl_Debut = pg_DebListeRDT;
                        break;
                     case XZECC_TYPE_EQT_DAI :
                        pl_Debut = pg_DebListeDAI;
                        break;
                     case XZECC_TYPE_EQT_PMV :
                        pl_Debut = pg_DebListePMV;
                        break;
                     case XZECC_TYPE_EQT_PMVA :
                        pl_Debut = pg_DebListePMVA;
                        break;
                     case XZECC_TYPE_EQT_BAF :
                        pl_Debut = pg_DebListeBAF;
                        break;
                     case XZECC_TYPE_EQT_BAD :
                        pl_Debut = pg_DebListeBAD;
                        break;
					      case XZECC_TYPE_EQT_PRV :
                        pl_Debut = pg_DebListePRV;
                        break;
                     case XZECC_TYPE_EQT_PIC :
                        pl_Debut = pg_DebListePIC;
                        break;
		     case XZECC_TYPE_EQT_CAP:
		     	pl_Debut = pg_DebListeCAP;
			break;
                     case XZECC_TYPE_EQT_PICTO :
                        pl_Debut = pg_DebListePICTO;
                        break;
                     case XZECC_TYPE_EQT_NAV :
                        pl_Debut = pg_DebListeNAV;
                        break;
                     case XZECC_TYPE_EQT_PAL :
                        pl_Debut = pg_DebListePAL;
                        break;
                     case XZECC_TYPE_EQT_GTC :
                        pl_Debut = pg_DebListeGTC;
                        break;
                     default : vl_ValRet = XDC_NOK;
                 }
         
                 /* Fermeture du fichiers de configuration LCR */
                 if ( (vl_ValRet == XDC_OK) && (pl_Debut != NULL) )
                 {
					/*A Recherche de l'equipement concerne */
                    for ( pl_EqtLCR = pl_Debut;
                          pl_EqtLCR != NULL;
                          pl_EqtLCR = pl_EqtLCR->Suivant )
                    {
                        if ( strlen(pl_EqtLCR->NomLT) > 0 )
                        {
                          sprintf ( pl_TexteConf, " %s,\t%d,\t%s,\n\0", 
                                           pl_EqtLCR->NomEqt,
                                           pl_EqtLCR->NumEqt,
                                           pl_EqtLCR->NomLT );
                          write ( vl_Fd, pl_TexteConf, strlen (pl_TexteConf) );
                          vl_FlagEcriture = 1;
                          XZST_03EcritureTrace ( XZSTC_WARNING, " NomEqt:%s NumEqt:%d NomLT:%s", 
                                           pl_EqtLCR->NomEqt,
                                           pl_EqtLCR->NumEqt,
                                           pl_EqtLCR->NomLT  );
                       }
                    }
                    close ( vl_Fd );
                 }
                 else
                 {
                   close ( vl_Fd );
                 }
                 
                 if ( !vl_FlagEcriture )
                   XZSS_06DetruireFichier ( pl_NomFichier );
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

int ec_liste_machine_lcr ( XZAOT_EqtSyn va_Eqt )

/*
* ARGUMENTS EN ENTREE :
*   Aucun
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
   char * 		pl_NomMachine;
		
    XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  ec_liste_machine_lcr: Num:%d Nom:%s",
    				va_Eqt.NumeroEqt,
    				va_Eqt.NomEqt );
    				
   pl_NomMachine = strtok ( va_Eqt.NomEqt, " " );
   for ( pl_Machine = pl_ExMachine = pm_ListeMachineLCR;
         pl_Machine != NULL;
         pl_ExMachine = pl_Machine, pl_Machine = pl_Machine->Suivant )
   {
       if ( strcmp (pl_NomMachine, pl_Machine->NomMachine) < 0 )
          break;
       if ( !strcmp (pl_NomMachine, pl_Machine->NomMachine) )
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
  	 return XDC_NOK;
      }
      else
      {
      	 memset ( pl_NewMachine, 0, sizeof(EINI_MACHINE) );
         strcpy ( pl_NewMachine->NomMachine, pl_NomMachine );
         if ( vl_existe != XDC_OK )
         {
            if ( pl_Machine == pm_ListeMachineLCR )
            {
                pl_NewMachine->Suivant    = pm_ListeMachineLCR;
                pm_ListeMachineLCR        = pl_NewMachine;
            }
            else
            {
               pl_NewMachine->Suivant = pl_ExMachine->Suivant;
               pl_ExMachine->Suivant  = pl_NewMachine;
            }
         }
      }
   }
   return XDC_OK;
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

void	ec_config_eqt_lcr (		XDY_NomMachine	pa_NomMachine,
				int		va_TypeEqt )

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine
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
*   Detruire les anciens fichiers
*   Creer les fichiers de config
*   Lire en base de donnees les parametres de configuration du type d'equipement concerne
*   Ecrire les donnees dans le fichier de configuration
*   Fermer les fichiers de configuration
*
------------------------------------------------------*/
{
  int			vl_indice = 0,
			vl_ValRet = XDC_OK,
			vl_EqtOk  = 0;
  char          	pl_PathEtFic[XDC_PATH_ABS_SIZE];
  XDY_Texte		pl_TexteConf;
  int			vl_Fd = 0;
  XDY_Entier		vl_result = 0;
  
   ec_config_lcr ( va_TypeEqt );
       
   /*A Creation de la liste des machine par appel de la primitive XZAO125 */
   if ( XZAO125_Liste_Eqt_Synoptique( EINI_NOM_BASE, NULL, NULL, XDC_EQT_MAC, ec_liste_machine_lcr ,&vl_result ) == XDC_NOK )
   {
       XZST_03EcritureTrace(   XZSTC_WARNING,  "Probleme lors de XZAO125_Liste_Eqt_Synoptique ");
   }

   vg_FdConf[0] = vg_FdConf[1] = vg_FdConf[2] = -1;
   
   /* Recherche de la machine concerne */
   for ( pm_MachineLCR = pm_ListeMachineLCR; pm_MachineLCR != NULL; pm_MachineLCR = pm_MachineLCR->Suivant )
   {
      /* Recherche du type de la machine (appel de XZSA_17) */
      XZSA_17TypeMachine ( pm_MachineLCR->NomMachine, &(pm_MachineLCR->TypeMachine) );      /* S'il s'agit de la machine concernee */

      if ( ((pm_MachineLCR->TypeMachine == XZSCC_TYPEM_SP) || (pm_MachineLCR->TypeMachine == XZSCC_TYPEM_LT) || 
	    (pm_MachineLCR->TypeMachine == XZSCC_TYPEM_PCS) || (pm_MachineLCR->TypeMachine == XZSCC_TYPEM_PC2) ||
	    (pm_MachineLCR->TypeMachine == XZSCC_TYPEM_PC2LT)) &&
           ( !strcmp (pm_MachineLCR->NomMachine, pa_NomMachine) ||
             !strcmp (pa_NomMachine, XZECC_TOUTES_MACHINES)) ) 
      {
         /* Recherche de l'equipement concerne */
         for ( vl_indice = 0; vl_indice < pg_SizeTabConf; vl_indice++)
         {
            /* S'il s'agit de l'equipement concerne */
            if ( ( pg_TabConf[vl_indice].TypeEqt == va_TypeEqt ) || ( va_TypeEqt == XZECC_TYPE_EQT_ALL ) )
	    {

               /* Selon le type d'equipement */
               switch ( pg_TabConf[vl_indice].TypeEqt )
               {
                  case XZECC_TYPE_EQT_RAU :
                     /* Appel de la fonction ei_config_eqt_rau du module MRAU */
                     vl_ValRet = ei_config_eqt_rau ( pm_MachineLCR );
                     break;
                   
                  case XZECC_TYPE_EQT_VID :
                     /* Appel de la fonction ei_config_eqt_video du module MVID */
                     //vl_ValRet = ei_config_eqt_video ( pm_MachineLCR );
                     break;
                  
                  case XZECC_TYPE_EQT_RDT :
                     /* Appel de la fonction ei_config_eqt_radt du module MRDT */
                     vl_ValRet = ei_config_eqt_radt ( pm_MachineLCR );
                     break;
                  
                  case XZECC_TYPE_EQT_DAI :
                     /* Appel de la fonction ei_config_eqt_dai du module MDAI */
                     vl_ValRet = ei_config_eqt_dai ( pm_MachineLCR );
                     break;
                  
                  case XZECC_TYPE_EQT_PMV :
                     /* Appel de la fonction ei_config_eqt_pmv du module MPMV */
                     vl_ValRet = ei_config_eqt_pmv ( pm_MachineLCR );
                     break;

                  case XZECC_TYPE_EQT_PMVA :
                     /* Appel de la fonction ei_config_eqt_pmva du module MPMVA */
                     vl_ValRet = ei_config_eqt_pmva ( pm_MachineLCR );
                     break;

                  case XZECC_TYPE_EQT_PICTO :
                     /* Appel de la fonction ei_config_eqt_picto du module MPMV */
                     vl_ValRet = ei_config_eqt_picto ( pm_MachineLCR );
                     break;
                  
                  case XZECC_TYPE_EQT_NAV :
                     /* Appel de la fonction ei_config_eqt_nav du module MNAV */
                     vl_ValRet = ei_config_eqt_nav ( pm_MachineLCR );
                     break;
                  
                  case XZECC_TYPE_EQT_BAF :
                     /* Appel de la fonction ei_config_eqt_baf du module MNAV */
                     vl_ValRet = ei_config_eqt_baf ( pm_MachineLCR );
                     break;
                  
                  case XZECC_TYPE_EQT_BAD :
                     /* Appel de la fonction ei_config_eqt_baf du module MNAV */
                     vl_ValRet = ei_config_eqt_bad ( pm_MachineLCR );
                     break;
					 
                  case XZECC_TYPE_EQT_PRV :
                     /* Appel de la fonction ei_config_eqt_pmva du module MPRV */
                     vl_ValRet = ei_config_eqt_prv ( pm_MachineLCR );
                     break;

                  case XZECC_TYPE_EQT_PIC :
                     /* Appel de la fonction ei_config_eqt_PIC du module MPIC */
                     vl_ValRet = ei_config_eqt_pic ( pm_MachineLCR );
                     break;
					 
		  case XZECC_TYPE_EQT_CAP:
		  	vl_ValRet = ei_config_eqt_cap(pm_MachineLCR);
			break;
                  case XZECC_TYPE_EQT_PAL :
                     /* Appel de la fonction ei_config_eqt_pal du module MPAL */
                     vl_ValRet = ei_config_eqt_pal ( pm_MachineLCR );
                     break;

                  case XZECC_TYPE_EQT_GTC :
                     /* Appel de la fonction ei_config_eqt_MiniGTC du module MGTC */
                     vl_ValRet = ei_config_eqt_MiniGTC ( pm_MachineLCR );
                     break;
               }
	    }
            if ( pg_TabConf[vl_indice].TypeEqt == va_TypeEqt ) break;
         }
      }
      
   }

/*!!!!!!!!!!!
   
   if ( (va_TypeEqt == XDC_EQT_GEN_LCR) || (va_TypeEqt == XZECC_TYPE_EQT_ALL) )
   {
      ei_config_Fentre_LCR_GEN_LCR ( pa_NomMachine );
   }
   if ( (va_TypeEqt == XDC_EQT_TDP) || (va_TypeEqt == XZECC_TYPE_EQT_ALL) )
   {
      ei_config_TDP_Fenetre_LCR ( pa_NomMachine );
   }
*/   
   
   pm_MachineLCR = NULL;
   
   /*A Cretion des fichiers pour fenetre LCR */
   ec_fichier_lcr ();  

   /*A Diffision des fichiers pour fenetre LCR */
   ei_diffusion_fichier_lcr ();

   ei_config_alerte_PAU_HS ( pm_ListeMachineLCR, pa_NomMachine );

}


