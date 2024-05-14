/*E*/
/*Fichier :  $Id: egab_application.c,v 1.2 2018/10/26 15:58:20 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/26 15:58:20 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_application.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de trace de la regulation >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/01/18 creation GABEB 1.1
* JMG	26/01/18 correction en tete 1.2
*
*
------------------------------------------------------*/

/* fichiers inclus */
#include "egab_application.h"
#include "egab_cfg.h"
#include "egab_util.h"
#include "egab_trace.h"
#include "xzat51.h"
#include "xzac303.h"
#include "xzac802.h"
#include "xzdd.h"
#include "xzaec.h"

extern XDY_NomSite              vg_NomSite;
extern XDY_NomMachine           vg_NomMachine;

int appli_init()
{
  return (XDC_OK);
}

int appli_PA(XDY_Evt va_evt, T_Liste_ECH *pa_ech) {
char vl_poste[20];
XDY_Horodate vl_HorodateSec;
        int  vl_JourSemaine;
T_CAB *pl_cab1;

if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
                                (int *) &vl_JourSemaine, (double *) &vl_HorodateSec ) != XDC_OK )
{
             XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_HorodateSec );
                return XDC_NOK;
}

			sprintf(vl_poste,"PO%s1",vg_NomSite);
	                XZDD02_CalculerPA(vl_HorodateSec,-1,vl_poste,va_evt,XZAEC_FMC_QueueBouchon,XZDDC_PA_SANS_TFM,XZDDC_MODE_OPERATIONNEL);
	return (XDC_OK);

pl_cab1 = pa_ech->ech.cab;

trace_process(EGAB_TRACE_TYPE_VALIDATION,
                        pa_ech->ech.echangeur.NumeroEchangeur,
                        va_evt.numero,va_evt.cle,
                        pl_cab1->config.NumeroCAP,
                        pa_ech->ech.contexte,
                        0,0,
                        "Calcul PA");
	
}
