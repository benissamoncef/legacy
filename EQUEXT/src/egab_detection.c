/*E*/
/*Fichier :  $Id: egab_detection.c,v 1.2 2018/10/26 15:59:07 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/26 15:59:07 $ ------------------------------------------------------
/* Fichier : @(#)ereg_detection.c       1.9      Release : 1.9        Date : 03/31/17
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEREG * FICHIER ereg_detection.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * < Module de general de detection de la regulation >
 *
------------------------------------------------------
 * HISTORIQUE :
* JMG	12/01/18 : creation GABEB 1.1
* JMG	26/01/18 : affinage traces 1.2
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include "egab_detection.h"
#include "egab_cfg.h"
#include "egab_util.h"
#include "egab_trace.h"
#include "xzit.h"

/* definitions de constantes */
#define CLE_TIMER_DETECTION             "cle_timer_detectionG"
#define C_MODE_SUD	XDC_SENS_SUD
#define C_MODE_NORD	XDC_SENS_NORD
#define C_MODE_CREATION_ONLY	1
#define C_MODE_ALL		2


XDY_Entier              vg_InitDetection = XDC_NOK;
extern XDY_NomSite              vg_NomSite;
extern XDY_NomMachine              vg_NomMachine;
extern T_Liste_CAB           *pg_CAB;
extern T_Liste_ECH           *pg_ech;

void det_bouchon_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);
void det_acquisition_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);
void null_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);
void detecte_bouchon(T_Liste_ECH *pl_ech, T_CAB *pl_cab1, XDY_Octet va_mode, int va_creation_only);
void det_etat_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  initialisation
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */
XDY_Entier det_init(char *va_scenarioDefaut){
        /*
         * ARGUMENTS EN ENTREE :
         *  Aucun
         *
         * ARGUMENTS EN SORTIE :
         *  Aucun
         *
         * CODE RETOUR :
         *  XDC_OK
         *  XDC_NOK
         *
         * CONDITION D'UTILISATION
         *  Doit etre invoque pour l'init du module
         *
         * FONCTION
         *
------------------------------------------------------*/
        XDY_Entier              vl_indice_callback, vl_JourSemaine_Msg;
        XDY_Datagroup   vl_nomDG, vl_DG_GABEB_DETECTION;
        XDY_Horodate    vl_horodate;

        XZST_03EcritureTrace( XZSTC_WARNING, "JBE JE RENTRE DANS :: det_init :: ");
        XZST_03EcritureTrace(XZSTC_FONCTION, "IN : det_init");

        if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
                        (int *) &vl_JourSemaine_Msg, (double *) &vl_horodate ) != XDC_OK )
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
        }
	XDG_EncodeDG2 ( vl_nomDG, XDG_GABEB_DETECTION, vg_NomSite) ;
        strcpy(vl_DG_GABEB_DETECTION, vl_nomDG);
        XZST_03EcritureTrace(XZSTC_INFO, "Abonnement au Datagroup %s", vl_nomDG );
        if ( !XDG_JoindreDG( vl_nomDG ) )
        {
                XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_nomDG );
                return XDC_NOK;
        }

        /*Abonnenement au DataGroup XDG_IETAT_DISPO_EQT*/
        XDG_EncodeDG2 ( vl_nomDG, XDG_IETAT_DISPO_EQT, vg_NomSite) ;
        if ( !XDG_JoindreDG( vl_nomDG ) )
        {
              XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_nomDG );
                }

	if(xzat27_Init_Etats_EqtsIhm(XDC_EQT_CAP,
                                     0,
                                     vl_nomDG) != XDC_OK)
        {
               XZST_03EcritureTrace(XZSTC_WARNING , "Probleme lors de l'initialisation des etats des equipements");
        }

       /*Abonnenement au DataGroup XDG_IETAT_INIEQT*/
      XDG_EncodeDG2 ( vl_nomDG, XDG_IETAT_INIEQT, vg_NomMachine) ;
                /*A joint ce datagroup */
                if ( !XDG_JoindreDG( vl_nomDG ) )
                {
                        XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_nomDG);
                }

	if (XZSM_01DemanderProgrammation(
                                vl_horodate+10,
                                10,
                                vl_DG_GABEB_DETECTION,
                                XDM_GABEB_DETECTION,
                                CLE_TIMER_DETECTION,
                                XZSMC_SEC_VRAI) != XDC_OK)
                {
                        XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Programmation du timer XDM_GABEB_DETECTION non faite." );
                        return XDC_NOK;
                }
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_GABEB_DETECTION),
				det_bouchon_cb,
							(T_CB_ARG)NULL) == NULL )
	{
			XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Armement Callback sur XDM_GABEB_DETECTION." );
			return XDC_NOK;
	}

	XDG_EncodeDG2 ( vl_nomDG, XDG_ETAT_CAP, vg_NomSite) ;
	XDG_JoindreDG( vl_nomDG );
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ETAT_CAP),
		det_etat_cb,
		 (T_CB_ARG)NULL) == NULL )
	{
                       XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Armement Callback sur XDM_ETAT_CAP." );
                       return XDC_NOK;
        }

	vg_InitDetection = XDC_OK;

        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : det_init");
        return XDC_OK;
}


void null_cb( T_IPC_CONN      va_Cnx,
                T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
                T_CB_ARG                        pa_ARG)
{
}

void det_etat_cb(T_IPC_CONN      va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
	T_CB_ARG                        pa_ARG)
{
#ifdef _TIBCO_EMS
   XDY_Mot 	vl_voie;
   XDY_Mot	vl_detection;
   XDY_Mot 	vl_numEqt_in;
#else
   T_INT2 	vl_voie;
   T_INT2	vl_detection;
   T_INT2 	vl_numEqt_in;
#endif

T_Liste_CAB	*pl_liste;

XZST_03EcritureTrace( XZSTC_FONCTION, "IN : det_etat_cb");
if (!TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2,&vl_numEqt_in,
		T_IPC_FT_INT2, &vl_voie,
		T_IPC_FT_INT2, &vl_detection,
		NULL))
{
	XZST_03EcritureTrace(XZSTC_WARNING, " Impossible de recuperer le message");
	return;
}

XZST_03EcritureTrace( XZSTC_FONCTION, "CAB %d voie %d etat %d",
			vl_numEqt_in, vl_voie, vl_detection);
pl_liste = pg_CAB;
for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
{
  if (pl_liste->cab.config.NumeroCAP == vl_numEqt_in)
  	break;
}

if (pl_liste==NULL) {
	XZST_03EcritureTrace(XZSTC_WARNING, " CAB %d non trouve ", vl_numEqt_in);
	return;
}
switch (vl_voie) {
	case 1 : pl_liste->cab.detection1 = vl_detection;
			break;
	case 2 : pl_liste->cab.detection2 = vl_detection;
			break;
	case 3 : pl_liste->cab.detection3 = vl_detection;
			break;
	case 4 : pl_liste->cab.detection4 = vl_detection;
			break;
}
XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : det_etat_cb");
}

void det_bouchon_cb(T_IPC_CONN                                    va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
		T_CB_ARG                        pa_ARG)
{
  T_Liste_ECH *pl_ech = NULL;
  T_CAB *pl_cab;
  int vl_mode = C_MODE_ALL;

  pl_ech = pg_ech;
  while (pl_ech !=NULL) {
      vl_mode = C_MODE_ALL;
	pl_cab = pl_ech->ech.cab;
	if (pl_cab!=NULL) {
      XZST_03EcritureTrace(XZSTC_FONCTION, "traitement echangeur %d sortie sud",
      			pl_ech->ech.echangeur.NumeroEchangeur);
	if ((pl_cab->detection3 == EGABC_DETECTION_HS) ||
	    (pl_cab->detection1 ==  EGABC_DETECTION_HS)) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "un cab HS => creation only");
	  vl_mode = C_MODE_CREATION_ONLY;
	}
        detecte_bouchon(pl_ech, pl_ech->ech.cab,
			C_MODE_SUD,vl_mode);
      
      vl_mode = C_MODE_ALL;
        XZST_03EcritureTrace(XZSTC_FONCTION, "traitement echangeur %d sortie nord",
      			pl_ech->ech.echangeur.NumeroEchangeur);
	pl_cab = pl_ech->ech.cab;
	if ((pl_cab->detection2 == XDC_FAUX) ||
	    (pl_cab->detection4 == XDC_FAUX)){
	  XZST_03EcritureTrace(XZSTC_FONCTION, "un cab HS => creation only");
	  vl_mode = C_MODE_CREATION_ONLY;
	}
        detecte_bouchon(pl_ech, pl_ech->ech.cab,
			C_MODE_NORD, vl_mode);
     } 
    pl_ech = pl_ech->suivant;
  }
  ajuste_analyse();
}
void detecte_bouchon(T_Liste_ECH *pa_ech, T_CAB *pa_cab,
		XDY_Octet va_mode, int va_creation_only) {

  XDY_Octet vl_etatd, vl_etatf;
  XDY_Octet vl_marche1, vl_marche2;
  XDY_Octet vl_detection=C_PAS_BOUCHON;
  XDY_Booleen vl_entree1, vl_entree2, vl_sortie1, vl_sortie2;
 T_CAB *pl_cab1;

      XZST_03EcritureTrace( XZSTC_FONCTION,"d1 %d // d2 %d // d3 %d // d4 %d",
		pa_cab->detection1, pa_cab->detection2, pa_cab->detection3, pa_cab->detection4);
      if (va_mode==C_MODE_SUD) {
      vl_etatf = pa_cab->detection3;
      vl_etatd = pa_cab->detection1;
	}
	else {
	vl_etatf = pa_cab->detection4;
	vl_etatd = pa_cab->detection2;
	}
            XZST_03EcritureTrace( XZSTC_DEBUG1,"etatd %d // etatf %d", vl_etatd, vl_etatf);
      if ((vl_etatd!=EGABC_DETECTION) && (vl_etatf!=EGABC_DETECTION))
        vl_detection = C_PAS_BOUCHON;
      else if ((vl_etatd==EGABC_DETECTION) && (vl_etatf==EGABC_NO_DETECTION))
        vl_detection = C_BOUCHON_BRETELLE;
      else if ((vl_etatd==EGABC_NO_DETECTION) && (vl_etatf==EGABC_DETECTION))
        vl_detection = C_BOUCHON_BOUT_BRETELLE;
      else if ((vl_etatd==EGABC_DETECTION) && (vl_etatf==EGABC_DETECTION))
        vl_detection = C_BOUCHON_BRETELLE_SC;
       XZST_03EcritureTrace(XZSTC_DEBUG1, "detection %d",vl_detection);
   
    /*recherche FMC fermeture echangeur en cours*/
    XZAE213_Fermeture_Echangeur(pa_ech->ech.echangeur.NumeroEchangeur,
    		&vl_entree1, &vl_entree2, &vl_sortie1, &vl_sortie2);

    if ((va_mode==C_MODE_SUD) && (vl_sortie1==XDC_OUI)) {
    	XZST_03EcritureTrace( XZSTC_FONCTION,"fermeture echangeur sortie 1 present=> pas de detection forcee");
	vl_detection = C_PAS_BOUCHON;
    }
    else if ((va_mode==C_MODE_NORD) && (vl_sortie2==XDC_OUI)) {
    	XZST_03EcritureTrace( XZSTC_FONCTION,"fermeture echangeur sortie 2 present=> pas de detection forcee");
	vl_detection = C_PAS_BOUCHON;
    }
    /*maj structure memoire*/
    if (va_mode==C_MODE_SUD)
      pa_ech->ech.detection_sud = vl_detection;
    else
      pa_ech->ech.detection_nord = vl_detection;

    if (vl_detection != C_PAS_BOUCHON) {
	pl_cab1 = pa_ech->ech.cab;

	if (va_mode==C_MODE_SUD)
		trace_process(EGAB_TRACE_TYPE_DETECTION,
			pa_ech->ech.echangeur.NumeroEchangeur,
			0,0,
			pl_cab1->config.NumeroCAP,
			vl_detection,
			0,0,
			"Detection bouchon bretelle sortie 1");
    	else
		trace_process(EGAB_TRACE_TYPE_DETECTION,
			pa_ech->ech.echangeur.NumeroEchangeur,
			 0,0,
			 pl_cab1->config.NumeroCAP,
			 vl_detection,
			 0,0,
			 "Detection bouchon bretelle sortie 2");
    }
}

