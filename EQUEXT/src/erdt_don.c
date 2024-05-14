/*E*/
/* Fichier : $Id: erdt_don.c,v 1.15 2011/03/17 15:38:53 gesconf Exp $        Release : $Revision: 1.15 $        Date : $Date: 2011/03/17 15:38:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  < Module de gestion des donnees de la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	08 Dec 1994	: Creation
* Volcic.F	version 1.2	26 Jan 1995	: Modif du code de retour de er_ins_liste_PtMes
* Volcic.F	version 1.3	28 Jan 1995	:
* Fontaine.C	version 1.4	22 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.5	12 Jun 1995	: Ajout er_ins_liste_Chaussee
* Volcic.F	version 1.6	09 Aou 1995	: Modif callback lt voisins
* Volcic.F	version 1.7	09 Aou 1995	: Modif callback lt voisins
* Volcic.F	version 1.8	09 Aou 1995	: Modif traces
* Volcic.F	version 1.9	27 Sep 1995	: Modif traces
* Volcic.F	version 1.10	14 Mai 1996	: Ajout mise a jour horodate pour 6gl pour test station avant extrapolation (DEM/1017)
* Volcic.F	version 1.11	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.12	27 Sep 1996	: Modif Extension Ouest suppression glissantes voisin (RADT_W)
* JPL	23/11/09	: Changement de niveaux de traces, notamment "Warning" en "Info" (DEM 908) 1.14
* JPL	17/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.15
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "erdt_don.h"
#include "erdt_tra.h"

EX_FONCT_INSERER   ( ERDT_DONNEES_RADT,er_ins_liste_RADT,pg_debutRADT,ConfRADT.NumeroStation,Suivant,ConfRADT)
EX_FONCT_SUPPRIMER ( ERDT_DONNEES_RADT,er_sup_liste_RADT,pg_debutRADT,ConfRADT.NumeroStation,Suivant)	
EX_FONCT_SUPPRIMER ( ERDT_DONNEES_PtMes,er_sup_liste_PtMes,pg_debutPtMes,ConfPtMes.NumeroStation,Suivant)	
EX_FONCT_SUPPRIMER ( ERDT_DONNEES_Chaussee,er_sup_liste_Chaussee,pg_debutChaussee,ConfChaussee.NumeroStation,Suivant)

/* definition de fonctions externes */

static char *version = "$Id: erdt_don.c,v 1.15 2011/03/17 15:38:53 gesconf Exp $      : erdt_don" ;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Fonction d'insertion dans la liste chainee des points de mesures 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    er_ins_liste_PtMes  (	 ERDT_DONNEES_PtMes *pa_Enr, int va_Taille ) 

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
*
* FONCTION
*
* Fonction d'insertion dans la liste chainee des points de mesures 
*
------------------------------------------------------*/
{ 
   ERDT_DONNEES_PtMes *pl_NewEnr, *pl_Enr, *pl_ExEnr; 
   int	vl_existe = XDC_NOK; 
   void *	vl_PtrSuiv;
	 
   for ( pl_Enr = pl_ExEnr = pg_debutPtMes; 
         pl_Enr != NULL; 
         pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant ) 
   { 
       if ( pa_Enr->ConfPtMes.NumeroStation < pl_Enr->ConfPtMes.NumeroStation ) 
          break; 
       if ( pa_Enr->ConfPtMes.NumeroStation == pl_Enr->ConfPtMes.NumeroStation && pa_Enr->ConfPtMes.Boucle == pl_Enr->ConfPtMes.Boucle ) 
       { 
          vl_existe = XDC_OK; 
          vl_PtrSuiv = pl_Enr->Suivant; 
          break; 
       } 
   } 
   if ( vl_existe == XDC_OK ) 
      pl_NewEnr = pl_Enr; 
   else 
      if ( ( pl_NewEnr = ( ERDT_DONNEES_PtMes *) malloc ( sizeof( ERDT_DONNEES_PtMes) ) ) == NULL) 
         return ( XDC_ECHEC ); 
   if ( va_Taille != 0 ) 
      memcpy ( &pl_NewEnr->ConfPtMes, &pa_Enr->ConfPtMes, va_Taille ); 
   else 
   { 
      memcpy ( pl_NewEnr, pa_Enr, sizeof( ERDT_DONNEES_PtMes) ); 
      pl_NewEnr->Suivant = vl_PtrSuiv; 
   } 
   if ( vl_existe != XDC_OK ) 
   { 
      if ( pl_Enr == pg_debutPtMes ) 
      { 
          pl_NewEnr->Suivant = pg_debutPtMes; 
          pg_debutPtMes           = pl_NewEnr; 
      } 
      else 
      { 
         pl_NewEnr->Suivant = pl_ExEnr->Suivant; 
         pl_ExEnr->Suivant  = pl_NewEnr; 
      } 
   } 
   
   if ( vl_existe == XDC_OK )
      return ( XDC_MODIF );
   else
      return ( XDC_AJOUT ); 

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Fonction d'insertion dans la liste chainee des chaussees 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    er_ins_liste_Chaussee  (	 ERDT_DONNEES_Chaussee *pa_Enr, int va_Taille ) 

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
*
* FONCTION
*
* Fonction d'insertion dans la liste chainee des chaussees 
*
------------------------------------------------------*/
{ 
   ERDT_DONNEES_Chaussee *pl_NewEnr, *pl_Enr, *pl_ExEnr; 
   int	vl_existe = XDC_NOK; 
   void *	vl_PtrSuiv;
	 
   for ( pl_Enr = pl_ExEnr = pg_debutChaussee; 
         pl_Enr != NULL; 
         pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant ) 
   { 
       if ( pa_Enr->ConfChaussee.NumeroStation < pl_Enr->ConfChaussee.NumeroStation ) 
          break; 
       if ( pa_Enr->ConfChaussee.NumeroStation == pl_Enr->ConfChaussee.NumeroStation && pa_Enr->ConfChaussee.Sens == pl_Enr->ConfChaussee.Sens ) 
       { 
          vl_existe = XDC_OK; 
          vl_PtrSuiv = pl_Enr->Suivant; 
          break; 
       } 
   } 
   if ( vl_existe == XDC_OK ) 
      pl_NewEnr = pl_Enr; 
   else 
      if ( ( pl_NewEnr = ( ERDT_DONNEES_Chaussee *) malloc ( sizeof( ERDT_DONNEES_Chaussee) ) ) == NULL) 
         return ( XDC_ECHEC ); 
   if ( va_Taille != 0 ) 
      memcpy ( &pl_NewEnr->ConfChaussee, &pa_Enr->ConfChaussee, va_Taille ); 
   else 
   { 
      memcpy ( pl_NewEnr, pa_Enr, sizeof( ERDT_DONNEES_Chaussee) ); 
      pl_NewEnr->Suivant = vl_PtrSuiv; 
   } 
   if ( vl_existe != XDC_OK ) 
   { 
      if ( pl_Enr == pg_debutChaussee ) 
      { 
          pl_NewEnr->Suivant = pg_debutChaussee; 
          pg_debutChaussee           = pl_NewEnr; 
      } 
      else 
      { 
         pl_NewEnr->Suivant = pl_ExEnr->Suivant; 
         pl_ExEnr->Suivant  = pl_NewEnr; 
      } 
   } 
   
   if ( vl_existe == XDC_OK )
      return ( XDC_MODIF );
   else
      return ( XDC_AJOUT ); 

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Fonction de remplissage des structures 6mn des mesures LT amont et aval 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    er_EMes_6mn_Voisin_cb (
        T_IPC_CONN                      va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG )

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
*  XDM_EMES_RADT_6gl
*
* FONCTION
*
* Fonction de remplissage des structures 6mn des mesures LT amont et aval 
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_don.c,v 1.15 2011/03/17 15:38:53 gesconf Exp $      : er_EMes_6mn_Voisin_cb";
   ERDT_DONNEES_RADT    *pl_DonCrt_RADT;
#ifdef _TIBCO_EMS
   XDY_Horodate       	vl_Horodate;
   XDY_Mot               vl_Numero, vl_Periode, vl_Sens, vl_Voie,
                        vl_MesQ, vl_MesV, vl_MesTO, vl_MesPL,
			vl_MesQ_Ind, vl_MesV_Ind, vl_MesTO_Ind, vl_MesPL_Ind;
#else
   T_REAL8       	vl_Horodate;
   T_INT2               vl_Numero, vl_Periode, vl_Sens, vl_Voie,
                        vl_MesQ, vl_MesV, vl_MesTO, vl_MesPL,
			vl_MesQ_Ind, vl_MesV_Ind, vl_MesTO_Ind, vl_MesPL_Ind;
#endif
   XZSMT_Horodate	pl_Date = "";
   XZSMT_Horodate	pl_date = "";
   XZSMT_Horodate	pl_Heure = "";
   
   int			vl_heures = 0, vl_minutes = 0, vl_secondes = 0;
   XDY_Horodate         vl_Horodate_R = 0;
   XDY_Entier           vl_Index_R = 0;
   XDY_Entier           vl_Index_R_Prec = 0;
   XDY_Mot              vl_Station_Vois = 0;
   XDY_Entier           vl_retcode = 0;
   XDY_Entier           vl_Resultat = XDC_OK;

   XZST_03EcritureTrace ( XZSTC_FONCTION,  "IN : er_EMes_6mn_Voisin_cb" );

   /*A Recuperation des parametres du message XDM_EMES_6mn_Voisin */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_REAL8, &vl_Horodate,
               T_IPC_FT_INT2,  &vl_Numero,
               T_IPC_FT_INT2,  &vl_Periode,
               T_IPC_FT_INT2,  &vl_Sens,
               T_IPC_FT_INT2,  &vl_Voie,
               T_IPC_FT_INT2,  &vl_MesQ,
               T_IPC_FT_INT2,  &vl_MesQ_Ind,
               T_IPC_FT_INT2,  &vl_MesV,
               T_IPC_FT_INT2,  &vl_MesV_Ind,
               T_IPC_FT_INT2,  &vl_MesTO,
               T_IPC_FT_INT2,  &vl_MesTO_Ind,
               T_IPC_FT_INT2,  &vl_MesPL,
               T_IPC_FT_INT2,  &vl_MesPL_Ind,
               NULL);

   XZST_03EcritureTrace ( XZSTC_INFO, "Horodate:%lf Numero:%d Periode:%d Sens:%d Voie:%d Q:%d InidQ:%d V:%d TO:%d PL:%d.",
                                      vl_Horodate, vl_Numero, vl_Periode, vl_Sens, vl_Voie,
                                      vl_MesQ, vl_MesQ_Ind, vl_MesV, vl_MesTO,  vl_MesPL );

   /*A Rangement des valeurs recues dans le tableau pg_ERDT_Mes_6gl */
   /*B Recherche de l'index en fonction du nombre minutes */
   for ( pl_DonCrt_RADT = pg_debutRADT; pl_DonCrt_RADT != NULL; pl_DonCrt_RADT = pl_DonCrt_RADT->Suivant )
   {
      if ( ( pl_DonCrt_RADT->ConfRADT.NumStationAmontS1 == vl_Numero ) ||
           ( pl_DonCrt_RADT->ConfRADT.NumStationAvalS1 == vl_Numero  ) ||
           ( pl_DonCrt_RADT->ConfRADT.NumStationAmontS2 == vl_Numero ) ||
           ( pl_DonCrt_RADT->ConfRADT.NumStationAvalS2 == vl_Numero  ) )
      {
          vl_Station_Vois = pl_DonCrt_RADT->ConfRADT.NumeroStation;
          break;
      }
   }
   
   if ( vl_Station_Vois == NULL ) return;
   
   XZSM_11ConversionHorodate ( vl_Horodate, &pl_Date );
   sscanf ( pl_Date, "%s %s", pl_date, pl_Heure );
   sscanf ( pl_Heure, "%02d:%02d:%02d", &vl_heures, &vl_minutes, &vl_secondes );
   
   if ( ( vl_retcode = er_recherche_index ( vl_Numero, vl_minutes, ERDTC_6MINUTE, &vl_Index_R, &vl_Index_R_Prec ) ) != XDC_OK )
   {
       XZST_03EcritureTrace( XZSTC_WARNING, "%s : er_recherche_index retourne %d", version, vl_retcode );
       XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : er_EMes_6mn_Voisin_cb" );
       return;
   }
   
   XZST_03EcritureTrace( XZSTC_INFO, "%s : INDEX = %d vl_numero=%d pg_Tab_RADT[vl_Numero]=%d",
                                     version, vl_Index_R,vl_Numero,pg_Tab_RADT[vl_Numero] );
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Horodate = vl_Horodate;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Voie = XDC_VOIE_SENS_NORMAL;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Mesures.ValQ.Val = vl_MesQ;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Mesures.ValQ.Indic = vl_MesQ_Ind;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Mesures.ValV.Val = vl_MesV;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Mesures.ValV.Indic = vl_MesV_Ind;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Mesures.ValTO.Val = vl_MesTO;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Mesures.ValTO.Indic = vl_MesTO_Ind;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Mesures.ValPL.Val = vl_MesPL;
   pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Numero]][vl_Sens][vl_Voie][vl_Index_R].Mesures.ValPL.Indic = vl_MesPL_Ind;
         
   XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : er_EMes_6mn_Voisin_cb %d", vl_Resultat );
}
