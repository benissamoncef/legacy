/*E*/
/* Fichier : $Id: etdc_tra.c,v 1.15 2011/03/17 12:45:47 gesconf Exp $      Release : $Revision: 1.15 $        Date : $Date: 2011/03/17 12:45:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDTC * FICHIER etdc_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de traitement des trames recues de la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	08 Dec 1994	: Creation
* Volcic.F	version 1.2	27 Jan 1995	: Modif Fichier equation 
* Volcic.F	version 1.3	14 Fev 1995	: 
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise historique
* Volcic.F	version 1.5	08 Jun 1995	: Ajout et_traitement
* Volcic.F	version 1.6	15 Nov 1995	: Modifs traces
* Volcic.F	version 1.7	17 Jun 1996	: Chercher fichier defaut dans EQUA a la place de DFT
* JMG   13/11/07        : ajout site de gestion DEM715 1.10
* JPL	28/09/09	: Filtrage messages de TERDT errones; ajout de traces (DEM 909) 1.11
* JPL	08/10/09	: Dimensions structures donnees et controles par constantes ERDT (DEM 909) 1.12
* JPL	08/10/09	: Message de mesures de TERDT: champs de flag et num. RADT plus larges (DEM 909) 1.13
* JPL	25/02/11 : Migration architecture HP ia64 (DEM 975) : retour de C.R. OK ou NOK (et_MAJ_Valeurs, et_trace_message)  1.14
* JPL	17/03/11 : Migration architecture HP ia64 (DEM 975) : adequation format scanf et type de donnee (bug)  1.15
------------------------------------------------------*/

/* fichiers inclus */

#include "etdc_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etdc_tra.c,v 1.15 2011/03/17 12:45:47 gesconf Exp $ : etdc_tra";

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern void et_sortir();

 
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Analyse de la trame LCR recue
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_MAJ_Valeurs ( XDY_Mot va_Index_RADT, XDY_Mot va_Index, XDY_Mot va_Index_Prec )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket : no socket sur laquelle le message est arriv≈
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*  analyse la trame et execute les actions correspondantes
*
------------------------------------------------------*/
{
   static char *version = "$Id: etdc_tra.c,v 1.15 2011/03/17 12:45:47 gesconf Exp $ : et_MAJ_Valeurs";
   int vl_index_tab = 0;
   int vl_Sens = 0;
   int vl_Voie = 0;
   
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN : %s", version );

   for ( vl_Sens = 1; vl_Sens < 9; (vl_Sens == 4) ? vl_Sens += 3 : vl_Sens ++ )
      for ( vl_Voie = 0; vl_Voie < 5; vl_Voie ++ )
      {
         pg_variables[vl_index_tab].valeur 		= pg_Mes_RADT[va_Index_RADT][vl_Sens][vl_Voie][va_Index_Prec].Mesures.ValV.Val;
         pg_variables[vl_index_tab + 1].valeur 		= pg_Mes_RADT[va_Index_RADT][vl_Sens][vl_Voie][va_Index].Mesures.ValV.Val;
         pg_variables[vl_index_tab + 60].valeur 	= pg_Mes_RADT[va_Index_RADT][vl_Sens][vl_Voie][va_Index_Prec].Mesures.ValQ.Val;
         pg_variables[vl_index_tab + 61].valeur 	= pg_Mes_RADT[va_Index_RADT][vl_Sens][vl_Voie][va_Index].Mesures.ValQ.Val;
         pg_variables[vl_index_tab + 120].valeur 	= pg_Mes_RADT[va_Index_RADT][vl_Sens][vl_Voie][va_Index_Prec].Mesures.ValTO.Val;
         pg_variables[vl_index_tab + 121].valeur 	= pg_Mes_RADT[va_Index_RADT][vl_Sens][vl_Voie][va_Index].Mesures.ValTO.Val;
         pg_variables[vl_index_tab + 180].valeur 	= pg_Mes_RADT[va_Index_RADT][vl_Sens][vl_Voie][va_Index_Prec].Mesures.ValPL.Val;
         pg_variables[vl_index_tab + 181].valeur 	= pg_Mes_RADT[va_Index_RADT][vl_Sens][vl_Voie][va_Index].Mesures.ValPL.Val;
         vl_index_tab += 2;
      }
             
   switch ( vg_Contexte )
   {
      case ETDCC_SEMAIME_JOUR :
         pg_variables[240].valeur = XDC_VRAI;
         pg_variables[241].valeur = XDC_FAUX;
         pg_variables[242].valeur = XDC_VRAI;
	 pg_variables[243].valeur = XDC_FAUX;
	 pg_variables[244].valeur = XDC_FAUX;
	 break;
	 
      case ETDCC_WEEKEND_JOUR :
         pg_variables[240].valeur = XDC_VRAI;
         pg_variables[241].valeur = XDC_FAUX;
         pg_variables[242].valeur = XDC_FAUX;
	 pg_variables[243].valeur = XDC_VRAI;
	 pg_variables[244].valeur = XDC_FAUX;
	 break;
	 
      case ETDCC_FERIE_JOUR :
         pg_variables[240].valeur = XDC_VRAI;
         pg_variables[241].valeur = XDC_FAUX;
         pg_variables[242].valeur = XDC_FAUX;
	 pg_variables[243].valeur = XDC_FAUX;
	 pg_variables[244].valeur = XDC_VRAI;
	 break;
	 
      case ETDCC_SEMAIME_NUIT :
         pg_variables[240].valeur = XDC_FAUX;
         pg_variables[241].valeur = XDC_VRAI;
         pg_variables[242].valeur = XDC_VRAI;
	 pg_variables[243].valeur = XDC_FAUX;
	 pg_variables[244].valeur = XDC_FAUX;
	 break;
	 
      case ETDCC_WEEKEND_NUIT :
         pg_variables[240].valeur = XDC_FAUX;
         pg_variables[241].valeur = XDC_VRAI;
         pg_variables[242].valeur = XDC_FAUX;
	 pg_variables[243].valeur = XDC_VRAI;
	 pg_variables[244].valeur = XDC_FAUX;
	 break;
	 
      case ETDCC_FERIE_NUIT :
         pg_variables[240].valeur = XDC_FAUX;
         pg_variables[241].valeur = XDC_FAUX;
         pg_variables[242].valeur = XDC_FAUX;
	 pg_variables[243].valeur = XDC_FAUX;
	 pg_variables[244].valeur = XDC_VRAI;
	 break;
   }

   pg_variables[245].valeur = vg_DIV[0];
   pg_variables[246].valeur = vg_DIV[1];
   pg_variables[247].valeur = vg_Cumul_Poids;   
   
   XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );

   return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Analyse de la trame LCR recue
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_traitement_trame ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket : no socket sur laquelle le message est arriv≈
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*  analyse la trame et execute les actions correspondantes
*
------------------------------------------------------*/
{
   static char *version = "$Id: etdc_tra.c,v 1.15 2011/03/17 12:45:47 gesconf Exp $ : et_traitement_trame";

   int				vl_IndCnx = 0;
   int				vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET		pl_Msg;
   char				*pl_Msg_Ptr = NULL;
   XDY_EMes_RADT		pl_Mesure_TDC[5][6];
   int				vl_DIV_sens1 = 0, vl_DIV_sens2 = 0;
   int				vl_retcode  = 0;
   int				vl_Flag_Recept = 0;
   int				vl_Index_Station  = 0;
   int				vl_NumRADT = 0;
   int				vl_Sens  = 0;
   int				vl_Index  = 0;
   int				vl_Index_Prec  = 0;
   int				vl_Chaussee  = 0;
   int				vl_Voie  = 0;
   ETDCT_VAR                    *pl_variables;
   XDY_FichEquext               pl_PathFic = "";
   FILE				*pl_Fd = NULL;
   XDY_Horodate			vl_Horodate = 0.0;
   int				vl_Contexte = 0;
   int				vl_ContexteSeuils = 0;
   int				vl_nb_valeurs = 0;
   
   XZST_03EcritureTrace( XZSTC_FONCTION, "IN : %s", version );
   
   /*A Lecture du message recu dans la socket appelante */
   if ( ( vl_retcode = ex_lire_msg ( va_socket, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) ) != XDC_OK )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "%s : Erreur lecture message socket. ", version );
      XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
      return (XDC_NOK);
   }
   else
   {
      /*A S'il s'agit d'une deconnexion */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      {
         /*B Arreter la tache par appel de et_sortir */
         XZST_03EcritureTrace( XZSTC_WARNING, "%s : Arret sur deconnexion de TERDT", version );
         et_sortir ();
      }

      /*!!! Trace du message recu (POUR DEBUG SEULEMENT) */
      /*!!! et_trace_message ( pl_Msg, vl_LgMsg );*/

      /*A Si la taille du message est superieure a celle d'un message de donnees alors */
      if ( vl_LgMsg > sizeof(pl_Mesure_TDC) )
      {	 
         /*A extraire les valeurs identifiant le message */
         vl_nb_valeurs = sscanf ( pl_Msg, "%2d%lf %02d%03d%1d%1d%1d%3d%3d",
                                     &vl_Flag_Recept, &vl_Horodate, &vl_Index_Station, &vl_NumRADT,
                                     &vl_Sens, &vl_Index, &vl_Index_Prec, &vl_DIV_sens1, &vl_DIV_sens1 ); 
         if ( ( vl_nb_valeurs == EOF )  ||  ( vl_nb_valeurs < 9 ) )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "%s : Message recu incorrect ( RADT %d, station %d) ",
                                     version, vl_NumRADT, vl_Index_Station );
   	    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_NOK);
         }
         XZST_03EcritureTrace( XZSTC_INFO, "%s : RADT %d, IdxStation=%d :: Sens=%d :: Index/IdxPrec=%d / %d",
                               version, vl_NumRADT, vl_Index_Station, vl_Sens, vl_Index, vl_Index_Prec );

         /*A Controler la validite des champs identifiant le message */
         if ( vl_Flag_Recept < 0 )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "%s : Type de mesure invalide (%d) pour RADT %d",
                                     version, vl_Flag_Recept, vl_NumRADT );
   	    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_NOK);
         }

         if ( ( vl_Index_Station < 0 ) || ( vl_Index_Station >= ERDTC_NB_STATION ) )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "%s : Valeur d'index station hors bornes (%d) pour RADT %d",
                                     version, vl_Index_Station, vl_NumRADT );
   	    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_NOK);
         }

         if ( ( vl_Sens < 0 ) || ( vl_Sens >= ERDTC_NB_CHAUSSEE ) )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "%s : Valeur de chaussee (sens) hors bornes (%d) pour RADT %d",
                                     version, vl_Sens, vl_NumRADT );
   	    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_NOK);
         }

         if ( ( vl_Index < 0 ) || ( vl_Index >= ERDTC_NB_INDEX_1MN ) )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "%s : Valeur d'index mesure hors bornes (%d) pour RADT %d",
                                     version, vl_Index, vl_NumRADT );
   	    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_NOK);
         }

         if ( ( vl_Index_Prec < 0 ) || ( vl_Index_Prec >= ERDTC_NB_INDEX_1MN ) )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "%s : Valeur d'index mesure precedente hors bornes (%d) pour RADT %d",
                                     version, vl_Index_Prec, vl_NumRADT );
   	    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_NOK);
         }

         /*A Si l'identifiant du message est correct alors extraire et memoriser les donnees */
         vg_DIV[0] = vl_DIV_sens1;
         vg_DIV[1] = vl_DIV_sens2;
         pl_Msg_Ptr = strstr ( pl_Msg, ETDCC_SEP_FLAG ) + strlen ( ETDCC_SEP_FLAG );
         memcpy( pl_Mesure_TDC, pl_Msg_Ptr, sizeof(pl_Mesure_TDC));
         memcpy ( pg_Mes_RADT[vl_Index_Station][vl_Sens], pl_Mesure_TDC, sizeof(pl_Mesure_TDC) ); 

         if ( vl_Flag_Recept != ETDCC_DERN_MSG_RDT )
         {
            XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_OK);
         }
      }   
      else
      {
         /*A sinon verifier si le message est un message de service */
         sscanf ( pl_Msg, "%1d%1d", &vl_Flag_Recept, &vl_Contexte );
         if ( vl_Flag_Recept == ETDCC_MSG_CTXT ) 
         {   
            /*A si c'est un message de changement de contexte alors le mettre a jour */
            vg_Contexte = vl_Contexte;
            XZST_03EcritureTrace( XZSTC_INFO, "%s : Mise a jour vg_Contexte = %d", version, vl_Contexte );
            XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_OK);
         }
         else
   	 {
   	    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
            return (XDC_NOK);
         }
      }

      /*A Remplissage de la structure de variables de sequations d'alerte => Appel et_MAJ_Valeurs */
      if ( et_MAJ_Valeurs ( vl_Index_Station, vl_Index, vl_Index_Prec ) != XDC_OK )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "%s : MAJ des valeurs recues impossibles => pas de calcul d'alerte", version );
         XZST_03EcritureTrace( XZSTC_WARNING, "OUT : %s", version );
         return (XDC_NOK);
      }

      /*A Ouverture du fichier XDF_CONF_EQU_ALERTE_R<vl_NumRADT> */
      /*! sprintf ( pl_PathFic, "%s/%s_R%02d", XZEXC_PATH_FIC_EQA, XDF_CONF_EQU_ALERTE, vl_NumRADT ); */
      sprintf ( pl_PathFic, "%s/%s_R%02d.txt", XZEXC_PATH_FIC_EQA, "equation_alerte", vl_NumRADT );
      XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : Ouverture Fichier %s", version, pl_PathFic);
      if ( ( pl_Fd = fopen ( pl_PathFic , "r" ) ) == NULL )
      { 
         XZST_03EcritureTrace( XZSTC_WARNING, "%s : fichier %s. introuvable", version, pl_PathFic );

         /*A Si le fichier XDF_CONF_EQU_ALERTE_R<vl_NumRADT> n'existe pas => ouverture du fichier par defaut */
         sprintf ( pl_PathFic, "%s/%s", XZEXC_PATH_FIC_EQA, XDF_CONF_EQU_ALERTE_DEFAUT );
         if ( ( pl_Fd = fopen ( pl_PathFic , "r" ) ) == NULL )
         {
             XZST_03EcritureTrace( XZSTC_WARNING, "%s : fichier %s. introuvable", version, pl_PathFic );
             XZST_03EcritureTrace( XZSTC_WARNING, " OUT : %s retourne XDC_NOK", version );
             return ( XDC_NOK );
         }
      }
      
      /*A Traitement du fichier d'equations d'alerte */
      if ( et_evaluer (    pl_Fd,
                           pg_variables,
                           ETDCC_NB_VAR,
                           vl_Horodate,
                           XDC_EQT_RAD,
                           vl_NumRADT,
                           XDC_NON)!=XDC_OK)
       {
          XZST_03EcritureTrace(XZSTC_WARNING,"%s echec de et_evaluer",version);
       }
       
       /*A Fermeture du fichier d'equations d'alertes */
       fclose(pl_Fd);
    }

    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
    return ( XDC_OK );   
}
 


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Trace brute d'un message recu (POUR DEBUG SEULEMENT).
*
--------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_trace_message ( XZEXT_MSG_SOCKET pa_Msg, int va_LgMsg )
{
   #define	CM_PATH_TRACE	"/tmp/tetdc/"
   #define	CM_FICH_TRACE	"message"

   static int	vl_compteur = 0;

   char		pl_nom_Fich_Trace[XDC_Lon_Texte];
   FILE		*pl_fTrace = NULL;

   /* la premiere fois supprimer les traces de la session precedente */
   if ( vl_compteur == 0 )
   {
      char vl_commande[XDC_Lon_Texte];

      sprintf ( vl_commande, "mkdir -p %s/old; rm -f %s/old/%s*; mv %s/%s* %s/old",
                   CM_PATH_TRACE, CM_PATH_TRACE, CM_FICH_TRACE, CM_PATH_TRACE, CM_FICH_TRACE, CM_PATH_TRACE );
      system (vl_commande);
   }

   vl_compteur++;
   sprintf ( pl_nom_Fich_Trace, "%s%s%d", CM_PATH_TRACE, CM_FICH_TRACE, vl_compteur );

   if ( ( pl_fTrace = fopen ( pl_nom_Fich_Trace, "w" ) ) != NULL )
   {
      fwrite ( pa_Msg, va_LgMsg, 1, pl_fTrace );
      fclose ( pl_fTrace );
   }

   return (XDC_OK);
}
