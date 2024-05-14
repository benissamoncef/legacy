/*E*/
/* Fichier : $Id: seta_act.h,v 1.3 2011/01/11 20:23:25 gesconf Exp $	Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 20:23:25 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETAT_ACT * FICHIER seta_act.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville   14 Oct 1994: Creation
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : Simple correction du nom des fonctions  1.3
******************************************************/

#ifndef seta_act
#define seta_act

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

int seta_actualiser_etamach(XZSCT_NomMachine pa_NomMachine,
                            int va_Etat);

int seta_actualiser_etassys(XZSCT_NomMachine pa_NomMachine,
                            XZSCT_NomSSysteme pa_NomSSysteme,
                            int va_Etat);

int seta_actualiser_etatach(XZSCT_NomMachine pa_NomMachine,
                            XZSCT_NomTache pa_NomTache,
                            int va_Etat);

int seta_actualiser_etaenti(XZSCT_NomEntite pa_NomEntite,
                            int va_TypeEntite,
                            int va_Etat);

int seta_actualiser_etamode(XZSCT_NomMachine pa_NomMachine,
		            int va_ModeIns,
	                    int va_ModeRec);

int seta_actualiser_etatrac(XZSCT_NomMachine pa_NomMachine,
                           XZSCT_NomTache pa_NomTache,
			   XZSTT_NiveauTrace va_NivTrace,
	                   int va_VisuTrace);
#endif
