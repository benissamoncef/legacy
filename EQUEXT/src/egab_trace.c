/*E*/
/* Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_trace.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de trace de la regulation >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/01/18	creation GABEB 1.1
------------------------------------------------------*/

/* fichiers inclus */
#include "egab_trace.h"
#include "xzst.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
int	vl_InitTrace = XDC_NOK;

/* declaration de fonctions internes */

/* declaration de variables externes */

int trace_init()
{
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : trace_init");

	vl_InitTrace = XDC_OK;

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : trace_init");
	return XDC_OK;
}



XDY_Entier trace_process(XDY_Octet va_type,
						XDY_Eqt va_ech,
						XDY_Entier va_numero_fmc,
						XDY_Entier va_cle_fmc,
						XDY_Eqt va_cab1,
						XDY_Octet va_contexte,
						XDY_Entier va_alerte,
						XDY_Octet va_acquittement,
				char* Texte, ...  )
{
	va_list		vl_listeArgument;
	XZAVT_ConfTraceCAB 	vl_confTrace;
	int 				vl_JourSemaine;
	XDY_Horodate 		vl_horodate;
	char vl_texteMotif[200];
	char tmp[1000];

	
	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
	                        (int *) &vl_JourSemaine, (double *) &vl_horodate ) != XDC_OK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_horodate );
	    return XDC_NOK;
	}
	XZST_03EcritureTrace( XZSTC_DEBUG1, "trace_process : recup horodate ok = %f", vl_horodate);

	vl_confTrace.Type = va_type;
	vl_confTrace.Cle_FMC = va_cle_fmc;
	vl_confTrace.Numero_FMC = va_numero_fmc;
	vl_confTrace.Echangeur = va_ech;
	vl_confTrace.Cab1 = va_cab1;
	vl_confTrace.Horodate = vl_horodate;
	vl_confTrace.Contexte = va_contexte;
	vl_confTrace.Alerte = va_alerte;
	vl_confTrace.Acquittement = va_acquittement;


    /* Initialisation du Pointeur d'argument sur le premier argument non nomme */
    memset (&vl_listeArgument,0,sizeof(va_list));
    va_start (vl_listeArgument, Texte);

    if (strlen(Texte) >= XZAVC_REG_LON_TRACE){
    	strncpy(vl_texteMotif, Texte, XZAVC_REG_LON_TRACE);
    	vl_texteMotif[XZAVC_REG_LON_TRACE-1]='\0';
    }
    else
    	strcpy(vl_texteMotif, Texte);

	XZST_03EcritureTrace( XZSTC_DEBUG1, "trace_process : texte avant recomposition =  \"%s\"", vl_texteMotif);
    vsprintf(tmp, vl_texteMotif , vl_listeArgument);
	XZST_03EcritureTrace( XZSTC_DEBUG1, "trace_process : texte apres recomposition =  \"%s\"", tmp);

    if (strlen(tmp) >= XZAVC_REG_LON_TRACE){
    	strncpy(vl_confTrace.Libelle,tmp, XZAVC_REG_LON_TRACE);
    	vl_texteMotif[XZAVC_REG_LON_TRACE-1]='\0';
    }
    else
    	strcpy(vl_confTrace.Libelle,tmp);

	XZST_03EcritureTrace( XZSTC_DEBUG1, "trace_process : Appel a XZAV01_Tracer_Regulation avec horodate=%f, message=\"%s\"",
									vl_confTrace.Type,
									vl_confTrace.Horodate,
									vl_confTrace.Libelle);

    if (XZAV04_Tracer_GABEB(vl_confTrace) == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_WARNING, "trace_process : Erreur lors de l'appel a XZAV02_Tracer_GABEB");
		return XDC_NOK;
	}
							   
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : trace_process");
	return XDC_OK;
}
