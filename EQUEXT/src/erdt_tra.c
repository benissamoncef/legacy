/*E*/
/* Fichier : $Id: erdt_tra.c,v 1.64 2021/07/12 06:05:03 gesconf Exp $        Release : $Revision: 1.64 $        Date : $Date: 2021/07/12 06:05:03 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de traitement des trames recues de la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	08 Dec 1994	: Creation
* Volcic.F	version 1.2	26 Jan 1995	: Ajout Calcul DIV moyenne
* Volcic.F	version 1.3	01 Fev 1995	: Ajout Trace Calcul DIV moyenne
* Volcic.F	version 1.4	15 Mar 1995	: MAJ Modifs
* Volcic.F	version 1.5	20 Mar 1995	: Modifs traitement nb boucles
* Volcic.F	version 1.6	20 Mar 1995	:
* Volcic.F	version 1.7	22 Mar 1995	:
* Volcic.F	version 1.8	19 Avr 1995	:
* Volcic.F	version 1.9	21 Avr 1995	: Reprise entete, historique
* Volcic.F	version 1.10	08 Jun 1995	: Ajout Message sockect avec TETDC
* Volcic.F	version 1.12	03 Jui 1995	: Modif stockage horodate MB et allocation dynamique du message recu
* Volcic.F	version 1.13	19 Jui 1995	: Creation d'une fonction commune de traitement des trames recues
* Volcic.F	version 1.14	27 Sep 1995	: Modifications traces
* Volcic.F	version 1.15	27 Sep 1995	: Modifications diverses
* Volcic.F	version 1.16	27 Sep 1995	: Modifications suite init
* Volcic.F	version 1.17	27 Sep 1995	: Modifications pour extrapolation peage
* Volcic.F	version 1.18	06 Nov 1995	: Modifications pour extrapolation peage
* Volcic.F	version 1.19	06 Nov 1995	: Ajout traces Dem. Individuelles
* Volcic.F	version 1.20	05 Dec 1995	: Modif Cmde LCR
* Volcic.F	version 1.21	23 Jan 1996	: Ajout presence fichier pour stockage erreur trame
* Volcic.F	version 1.22	07 Mai 1996	: Ajout fonction er_mesures_valides pour test station avant extrapolation (DEM/1017)
* Volcic.F	version 1.23	14 Mai 1996	: Correction %PL pb sur correction (DEM/1015)
* Volcic.F	version 1.24	14 Mai 1996	: Correction er_mesures_valides pb sur correction (DEM/1017)
* Mismer.D	version 1.25	30 Mai 1996	: Ajout appel fonction compteur de marche XZMA01 (DEM/643)
* Volcic.F	version 1.18	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.19	18 Sep 1996	: Modif Extension Ouest Suite (RADT_W)
* Mismer.D	version 1.20	26 Sep 1996	: Modif Extension Ouest suppression des DIV (RADT_W)
* Mismer.D	version 1.21	02 Sep 1996	: Modif pour mesures pesage (DEM/1227)
* Mismer.D	version 1.22	09 Sep 1996	: Modif pour mesures individuelle avec pesage (DEM/1227)
* Mismer.D	version 1.23	09 Sep 1996	: Modif pour mesures individuelle avec format longueur veh (DEM/1227)
* Mismer.D	version 1.24	09 Sep 1996	: Modif pour envoi mesure extrapol� (DEM/1227)
* Mismer.D	version 1.33	29 Oct 1996	: Correction sens voie sur LT (DEM/1227)
* Mismer.D	version 1.34	29 Nov 1996	: Correction alarme port serie (DEM/1288)
* Mismer.D	version 1.35	16 Dec 1996	: Modif rafraichissement syno sur reprise com (DEM/1291/1)
* Mismer.D	version 1.36	16 Dec 1996	: Suite Modif rafraichissement syno sur reprise com (DEM/1291/1)
* Mismer.D	version 1.37	27 Jan 1997	: Modif calcul DIV su MI (DEM/1392)
* Mismer.D	version 1.38	10 Fev 1997	: Modif TDP (decodage status )(DEM/1395)
* Mismer.D	version 1.39	07 Nov 1997	: Ajout traitement partiel des trame historiques (DEM/xxxx)
* Hilmarcher    version 1.43    22 juin 2005    : inhibition de l'extrapolation des donnes 6mn DEM476
* Niepceron	version 1.44	25 Aout 2005	: Duree de collecte des donn�es individuelles augment�e � 12h (43200sec) DEM503
* Niepceron	version 1.45	13 Sep 2007	: Ne pas tenter d'extraire les donn�es comptage veh (calcul %PL)
* 						   de la trame MB LC pour les boucles sur bretelles DEM703
* JMG		13/11/07 : ajout site de gestion DEM715 1.46
* JPL		08/10/09 : Message de mesures a TETDC: champs de flag et num. RADT plus larges (DEM 909) 1.47
* JPL		23/11/09 : Changement de niveaux de traces, notamment "Warning" en "Info" (DEM 908) 1.48
* JPL		23/11/09 : Suppression envoi superflu mesure 6mn a TISYN 1.49
* JPL		17/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.50
* JPL		17/03/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=,+,-)  1.51
* JPL		19/12/11 : Initialisation de toutes les mesures a "INCONNU" dans tous les cas (DEM 997)  1.52
* JMG		04/07/12 : qualification DEM/1016
* JPL		18/12/12 : Pas de reinitialisation des mesures PL si elles ne sont pas extrapolees  1.54
* JMG		15/07/13 : modiftraces
* JMG/PNI		12/11/18 : ajout IP / prise en compte du type de station sans trame inverse pour Toulon  DEM1306 1.57
* JMG		14/05/19 : correction recherche_eqt num_ls si IP 1.58
* AMI    28/01/21 : SAE_191: chantier date: remplacement de time pas XZSM07
* JPL	23/02/21	: SAE-243 : Crrectif recup donn�es histo x 10
* PNI	5/7/21		:SAE 308 : correctif trame reduite bretelles
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "xzma.h"

#include "ex_mala.h"
#include "ex_msta.h"

#include "erdt_don.h"
#include "erdt_cfg.h"
#include "erdt_cmd.h"
#include "erdt_tra.h"
#include "xzsm.h"

/* definitions de constantes */

/* definitions de types locaux */
#define C_FILE_SIMU "/produits/migrazur/appliSD/fichiers/dyn/simu_rdt.txt"

/* definition de macro locales */
extern XDY_Booleen vg_mode_simu;
static XZSMT_Horodate vm_horo_simu;
static FILE *vm_fd_file_simu = NULL;
static int vm_lignes_simu = 0;
/* declaration de variables locales */

static char *version = "$Id: erdt_tra.c,v 1.64 2021/07/12 06:05:03 gesconf Exp $ : erdt_tra.c";

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern void trim(char *Ligne)
{
	int i = 0;
	int j = 0;
	while (Ligne[j] == ' ')
		j++;
	for (i = j; i < strlen(Ligne); i++)
		Ligne[i - j] = Ligne[i];
	Ligne[i - j] = '\0';
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Recherche de l'index du tableau de structures des mesures RADT
*
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void er_lit_fichier_simu(XDY_Horodate va_horodate)

/*
 * ARGUMENTS EN ENTREE :
 *
 * XDY_Eqt		va_Num_RADT
 * XDY_Mot		va_Nb_Minutes
 *
 * ARGUMENTS EN SORTIE :
 *
 * CODE RETOUR :
 *
 * XDC_OK
 * XDC_NOK
 *
 * CONDITION D'UTILISATION
 *
 * FONCTION
 *
 *  Recherche de l'index du tableau de structures des mesures RADT
 *
------------------------------------------------------*/
{
	char pl_Ligne[200];
	int vl_retcode;
	int vl_Num_RADT;
	int vl_old_radt;
	int vl_minutes;
	XDY_Entier vl_Index, vl_Index_Prec;
	XZSMT_Horodate vl_date;
	XDY_Datagroup pl_DG = "";
	ERDT_DONNEES_RADT *pl_ListeRADT;
	struct tm vl_Enr;
	int vl_Chaussee;
	int vl_ChausseeIndex;
	int vl_Voie;
	int i = 0;
	XDY_Entier vl_Q = 0;
	XDY_Entier vl_Q_inv = 0;
	XDY_Entier vl_V = 0;
	XDY_Entier vl_V_inv = 0;
	XDY_Entier vl_TO = 0;
	XDY_Entier vl_TO_inv = 0;
	char vl_str_num[50];
	char vl_str_sens[50];
	char vl_str_voie[50];
	char vl_str_horo[50];
	char vl_str_Q[50];
	char vl_str_Q_inv[50];
	char vl_str_TO[50];
	char vl_str_TO_inv[50];
	char vl_str_V[50];
	char vl_str_V_inv[50];
	long int vl_pos;
	long int vl_pos2;

	XZST_03EcritureTrace(XZSTC_FONCTION, " IN : er_lit_fichier_simu");

	vm_fd_file_simu = fopen(C_FILE_SIMU, "r");
	strcpy(vm_horo_simu, "");
	vl_old_radt = 0;
	if (vm_fd_file_simu == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "er_lit_fichier_simu : erreur ouverture fichier %s", C_FILE_SIMU);
		return;
	}
	/*premiere et secondes lignes a virer*/
	fgets(pl_Ligne, sizeof(pl_Ligne), vm_fd_file_simu);
	fgets(pl_Ligne, sizeof(pl_Ligne), vm_fd_file_simu);

	XZSM_11ConversionHorodate(va_horodate, &vl_date);
	XZST_03EcritureTrace(XZSTC_INFO, "%s -- %d", vl_date, vm_lignes_simu);
	sscanf(vl_date, "%02u/%02u/%04u %02u:%02u",
		   &vl_Enr.tm_mday,
		   &vl_Enr.tm_mon,
		   &vl_Enr.tm_year,
		   &vl_Enr.tm_hour,
		   &vl_minutes);

	XZST_03EcritureTrace(XZSTC_INFO, "horo simu %s // minutes %d", vm_horo_simu, vl_minutes);

	/*ne pas traiter les lignes deja traitees dans un cycle precedent*/
	for (i = 0; i < vm_lignes_simu; i++)
		fgets(pl_Ligne, sizeof(pl_Ligne), vm_fd_file_simu);

	/*on attaque les lignes a traiter*/
	do
	{

		if (fgets(pl_Ligne, sizeof(pl_Ligne), vm_fd_file_simu) == NULL)
			return;
		vm_lignes_simu++;

		XZST_03EcritureTrace(XZSTC_INFO, "ligne %s", pl_Ligne);
		/*decodage de la chaine*/
		/*recup horodate de la donnee, numero de station, sens, voie + les valeurs et leurs indicateurs*/
		/*#num#sens#voie#horo#Q#Q ind#V#V ind#TO#TO ind*/
		sscanf(pl_Ligne, "#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#",
			   vl_str_num,
			   vl_str_sens,
			   vl_str_voie,
			   vl_str_horo,
			   vl_str_Q,
			   vl_str_Q_inv,
			   vl_str_V,
			   vl_str_V_inv,
			   vl_str_TO,
			   vl_str_TO_inv);
		/*virer les espaces au debut*/
		trim(vl_str_num);
		trim(vl_str_sens);
		trim(vl_str_voie);
		trim(vl_str_horo);
		trim(vl_str_Q);
		trim(vl_str_Q_inv);
		trim(vl_str_V);
		trim(vl_str_V_inv);
		trim(vl_str_TO);
		trim(vl_str_TO_inv);
		if (!strcmp(vm_horo_simu, ""))
			strcpy(vm_horo_simu, vl_str_horo);

		XZST_03EcritureTrace(XZSTC_INFO, "%s//%s//%s//%s//%s//%s//%s//%s//%s//%s",
							 vl_str_num, vl_str_sens, vl_str_voie, vl_str_horo, vl_str_Q,
							 vl_str_Q_inv, vl_str_V, vl_str_V_inv, vl_str_TO, vl_str_TO_inv);
		XZST_03EcritureTrace(XZSTC_WARNING, "horo simu %s", vm_horo_simu);
		vl_Num_RADT = atoi(vl_str_num);
		vl_Chaussee = atoi(vl_str_sens);
		vl_Voie = atoi(vl_str_voie);
		if (strcmp(vl_str_Q, "NULL"))
			vl_Q = atoi(vl_str_Q);
		else
			vl_Q = -1;
		vl_Q_inv = atoi(vl_str_Q_inv);
		vl_V_inv = atoi(vl_str_V_inv);
		if (strcmp(vl_str_V, "NULL"))
			vl_V = atoi(vl_str_V);
		else
			vl_V = -1;
		if (strcmp(vl_str_TO, "NULL"))
			vl_TO = atoi(vl_str_TO);
		else
			vl_TO = -1;
		vl_TO_inv = atoi(vl_str_TO_inv);

		if ((vl_retcode = er_recherche_index(vl_Num_RADT, vl_minutes, ERDTC_6MINUTE, &vl_Index, &vl_Index_Prec)) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "er_lit_fichier_simu : er_recherche_index retourne %d", vl_retcode);
			return;
		}

		if (vl_old_radt == 0)
			vl_old_radt = vl_Num_RADT;

		if (!strcmp(vl_str_horo, vm_horo_simu) && (vl_old_radt == vl_Num_RADT))
		{
			XZST_03EcritureTrace(XZSTC_INFO, "RADT %d %d %d %d", vl_Num_RADT, vl_Chaussee, vl_Voie, vl_Index);
			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Horodate = va_horodate;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValQ.Val = vl_Q;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValQ.Indic = vl_Q_inv;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValTO.Val = vl_TO;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValTO.Indic = vl_TO_inv;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValV.Val = vl_V;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValV.Indic = vl_V_inv;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Voie = XDC_VOIE_SENS_NORMAL;
		}

		/*changement de station de comptage dans le fichier*/
		/* ou d horodate*/
		if ((vl_old_radt != vl_Num_RADT) || (strcmp(vl_str_horo, vm_horo_simu)))
		{
			XZST_03EcritureTrace(XZSTC_INFO, "calcul chaussee");

			if ((vl_retcode = er_Calcul_Chaussee(vl_old_radt, ERDTC_CHAUSSEE, vl_Index, va_horodate)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "6min QVTO : er_Calcul_Chaussee retourne %d", vl_retcode);

				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Horodate = va_horodate;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_Chaussee][0][vl_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
			}

			for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
				if (pl_ListeRADT->ConfRADT.NumeroStation == vl_old_radt)
				{
					break;
				}

			sprintf(pl_DG, "%s_%s", XDG_EMES_RADT_6MN, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion));
			for (vl_ChausseeIndex = 1; vl_ChausseeIndex < ERDTC_NB_CHAUSSEE; vl_ChausseeIndex++)
			{
				if (!TipcSrvMsgWrite(pl_DG, XDM_IdentMsg(XDM_EMES_6mn_Chaussee), XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, va_horodate, T_IPC_FT_INT2, vl_old_radt, T_IPC_FT_INT2, ERDTC_6MINUTE, T_IPC_FT_INT2, vl_ChausseeIndex, T_IPC_FT_INT2, 0, T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValQ.Val, T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValQ.Indic, T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValV.Val, T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValV.Indic, T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValTO.Val, T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValTO.Indic, T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValPL.Val, T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValPL.Indic, NULL))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : Envoi message XDM_EMES_6mn_Chaussee non effectue ", version);
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_WARNING,
										 "%s : Envoi message XDM_EMES_6mn_Chaussee sur %s effectue avec succes : %d %d", version,
										 pl_DG,
										 pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValQ.Val,
										 pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_old_radt]][vl_ChausseeIndex][0][vl_Index].Mesures.ValTO.Val);
				}
				/*B Envoi du message */
				TipcSrvFlush();
			}
			if (!strcmp(vl_str_horo, vm_horo_simu) && (vl_old_radt != vl_Num_RADT))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "RADT %d %d %d %d", vl_Num_RADT, vl_Chaussee, vl_Voie, vl_Index);
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Horodate = va_horodate;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValQ.Val = vl_Q;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValQ.Indic = vl_Q_inv;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValTO.Val = vl_TO;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValTO.Indic = vl_TO_inv;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValV.Val = vl_V;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValV.Indic = vl_V_inv;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][vl_Voie][vl_Index].Voie = XDC_VOIE_SENS_NORMAL;
			}
			vl_old_radt = vl_Num_RADT;
		}
	} while (!strcmp(vl_str_horo, vm_horo_simu));
	vm_lignes_simu--;
	strcpy(vm_horo_simu, vl_str_horo);

	fclose(vm_fd_file_simu);
	XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : er_lit_fichier_simu");
	return;
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *
 * Recherche de l'index du tableau de structures des mesures RADT
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int er_recherche_index(XDY_Eqt va_Num_RADT,
					   XDY_Mot va_Nb_Minutes,
					   XDY_Mot va_Type_Donnees,
					   XDY_Entier *pa_Index_Tab,
					   XDY_Entier *pa_Index_Tab_Prec)
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Eqt		va_Num_RADT
* XDY_Mot		va_Nb_Minutes
*
* ARGUMENTS EN SORTIE :
*
* XDY_Entier	pa_Index_Tab
* XDY_Entier	pa_Index_Tab_Prec
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*  Recherche de l'index du tableau de structures des mesures RADT
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_tra.c,v 1.64 2021/07/12 06:05:03 gesconf Exp $ : er_recherche_index";

	ldiv_t vl_Result_Div;

	XZST_03EcritureTrace(XZSTC_FONCTION, " IN : er_recherche_index RADT = %d\tNb Min = %d\tType = %d ", va_Num_RADT, va_Nb_Minutes, va_Type_Donnees);

	if (va_Num_RADT <= 0)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "er_recherche_index : RADT %d incorrecte", va_Num_RADT);
		return (XDC_NOK);
	}
	else
	{
		vl_Result_Div = ldiv(va_Nb_Minutes, 6);

		if (va_Type_Donnees == ERDTC_MINUTE)
		{
			*pa_Index_Tab = (XDY_Entier)vl_Result_Div.rem;
			switch (vl_Result_Div.rem)
			{
			case 0:
				*pa_Index_Tab_Prec = (XDY_Entier)vl_Result_Div.rem + 5;
				break;

			default:
				*pa_Index_Tab_Prec = *pa_Index_Tab - 1;
				break;
			}
		}
		else
		{
			if (va_Type_Donnees == ERDTC_6MINUTE)
			{
				*pa_Index_Tab = (XDY_Entier)vl_Result_Div.quot;
				switch (vl_Result_Div.quot)
				{
				case 0:
					*pa_Index_Tab_Prec = (XDY_Entier)vl_Result_Div.quot + 9;
					break;

				default:
					*pa_Index_Tab_Prec = *pa_Index_Tab - 1;
					break;
				}
			}
			else
				return (XDC_NOK);
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : er_recherche_index Index = %d\tIndex Prec = %d", *pa_Index_Tab, *pa_Index_Tab_Prec);
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Recherche de l'index des dernieres mesures RADT stockees
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_dernier_index(XDY_Eqt va_Num_RADT,
					 XDY_Entier va_Duree,
					 XDY_Entier *pa_Index_Tab,
					 XDY_Horodate *pa_Horodate)
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Eqt		va_Num_RADT
*
* ARGUMENTS EN SORTIE :
*
* XDY_Entier	pa_Index_Tab
* XDY_Horodate	pa_Horodate
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*  Recherche de l'index du tableau des dernieres mesures RADT stockees
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_tra.c,v 1.64 2021/07/12 06:05:03 gesconf Exp $ : er_dernier_index";

	ERDT_DONNEES_PtMes *pl_DonCrt_PtMes;
	XDY_Entier vl_i = 0;
	XDY_Entier vl_Chaussee = 0;
	XDY_Entier vl_FinBoucle = 0;
	XDY_Entier vl_Voie = 0;
	XDY_Horodate vl_Horodate = 0.0;
	XDY_Horodate vl_Anc_Horodate = 0.0;
	XDY_Booleen vl_Existe = XDC_FAUX;

	XZST_03EcritureTrace(XZSTC_FONCTION, " IN : er_dernier_index RADT = %d", va_Num_RADT);

	*pa_Index_Tab = ERDTC_INDEX_INCONNU;
	*pa_Horodate = 0.0;

	if (va_Num_RADT <= 0)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "er_dernier_index : RADT n� %d incorrecte", va_Num_RADT);
		XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : er_dernier_index Index = %d", *pa_Index_Tab);
		return (XDC_NOK);
	}
	else
	{
		/*A Recherche d'une chaussee et d'un point de mesure valide de la station RADT */
		for (pl_DonCrt_PtMes = pg_debutPtMes; pl_DonCrt_PtMes != NULL; pl_DonCrt_PtMes = pl_DonCrt_PtMes->Suivant)
		{
			if (pl_DonCrt_PtMes->ConfPtMes.NumeroStation == va_Num_RADT)
			{
				if (pl_DonCrt_PtMes->EtatVoie == XDC_VOIE_SENS_NORMAL || pl_DonCrt_PtMes->EtatVoie == XDC_VOIE_SENS_INVERSE)
				{
					vl_Chaussee = pl_DonCrt_PtMes->ConfPtMes.Sens;
					vl_Voie = pl_DonCrt_PtMes->ConfPtMes.Voie;
					vl_Existe = XDC_VRAI;
					break;
				}
			}
		}

		if (vl_Existe == XDC_FAUX)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "er_dernier_index : Pas de points de mesures valides pour la station RADT n� %d", va_Num_RADT);
			XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : er_dernier_index Index = %d\t Horodate = %d", *pa_Index_Tab, *pa_Horodate);
			return (XDC_NOK);
		}

		/*A Recherche du dernier index */
		switch (va_Duree)
		{
		case ERDTC_MINUTE:
			for (vl_i = 0; vl_i < ERDTC_NB_INDEX_1MN; vl_i++)
			{
				vl_Horodate = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_i].Horodate;
				if (vl_Horodate > vl_Anc_Horodate)
				{
					*pa_Index_Tab = vl_i;
					vl_Anc_Horodate = vl_Horodate;
				}
			}
			break;

		case ERDTC_6MINUTE:
			for (vl_i = 0; vl_i < ERDTC_NB_INDEX_6MN; vl_i++)
			{
				vl_Horodate = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_i].Horodate;
				if (vl_Horodate > vl_Anc_Horodate)
				{
					*pa_Index_Tab = vl_i;
					vl_Anc_Horodate = vl_Horodate;
				}
			}
			break;
		}
		if (*pa_Index_Tab == ERDTC_INDEX_INCONNU)
			*pa_Index_Tab = ERDTC_INDEX_DEFAUT;
		*pa_Horodate = vl_Anc_Horodate;
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : er_dernier_index Index = %d\t Horodate = %d", *pa_Index_Tab, *pa_Horodate);
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Recherche si les dernieres mesures RADT stockees sont valident
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_mesures_valides(XDY_Eqt va_Num_RADT, XDY_Sens va_Sens)
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Eqt		va_Num_RADT
* XDY_Sens		va_Sens
*
* ARGUMENTS EN SORTIE :
*
* aucun
*
* CODE RETOUR :
*
* XDC_OK  mesures valables
* XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*  Recherche si les mesures en memoires sont fraiches
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_tra.c,v 1.64 2021/07/12 06:05:03 gesconf Exp $ : er_mesures_valides";

	XDY_Entier vl_i = 0;
	XDY_Entier vl_index = -1;
	XDY_Entier vl_Voie = 0;
	XDY_Horodate vl_Horodate = 0.0;
	XDY_Horodate vl_Anc_Horodate = 0.0;
	//	time_t				vl_time = 0;
	int vl_jourSemaine = 0;

	XZST_03EcritureTrace(XZSTC_FONCTION, " IN : er_mesures_valides RADT = %d", va_Num_RADT);

	if (va_Num_RADT <= 0)
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : er_mesures_valides retourne NOK car num station errone ");
		return (vl_index);
	}
	else
	{
		for (vl_i = 0; vl_i < ERDTC_NB_INDEX_6MN; vl_i++)
			for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Sens][vl_Voie][vl_i].Voie == XDC_VOIE_SENS_NORMAL) &&
					(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Sens][vl_Voie][vl_i].Mesures.ValQ.Indic == XDC_RADT_CORRECT))
				{
					vl_Horodate = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Sens][vl_Voie][vl_i].Horodate;
					if (vl_Horodate > vl_Anc_Horodate)
					{
						vl_Anc_Horodate = vl_Horodate;
						vl_index = vl_i;
					}
				}

		/*	time ( &vl_time );
			vl_Horodate = (XDY_Horodate) vl_time;remplacer par XZSM_07 XZAE_191*/
		XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate);

		/*A Si les mesures datent de moins de 2.5 mn alors retourne OK sinon NOK */
		if ((vl_Horodate - vl_Anc_Horodate > 480.0) || (vl_Anc_Horodate - vl_Horodate > 300.0))
		{
			XZST_03EcritureTrace(XZSTC_FONCTION, " OUT :  er_mesures_valides retourne NOK ");
			return (-1);
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_FONCTION, " OUT :  er_mesures_valides retourne OK ");
			return (vl_index);
		}
	}
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Recherche d'un equipement en memoire
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int er_recherche_eqt(char *va_adrrgs,
					 XDY_Entier va_typ_rep,
					 XDY_Mot *va_noeqt,
					 XDY_Mot *va_num_ls,
					 XDY_Mot *va_typestation,
					 XDY_Horodate *va_horodate,
					 XDY_Entier *va_Etat_Eqt,
					 XDY_District *va_SiteGestion,
					 ERDT_DONNEES_RADT *pa_station)
/*
* ARGUMENTS EN ENTREE :
*
* va_adrrgs : adresse rgs de l'eqt
* va_typ_rep : type de reponse
*
* ARGUMENTS EN SORTIE :
*
* va_noeqt : no eqt trait�
*
* CODE RETOUR :
*
* XDC_FAUX : eqt non trouve
* XDC_EQT_RAD : eqt RADT trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*  Recherche d'un equipement en memoire
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_tra.c,v 1.64 2021/07/12 06:05:03 gesconf Exp $ : er_recherche_eqt";

	ERDT_DONNEES_RADT *pl_ListeRADT;
	ERDT_DONNEES_RADT vl_station;
	int vl_typeeqt = 0;
	int vl_num_ls = 0;
	char *vl_status[32];

	XZST_03EcritureTrace(XZSTC_WARNING, "er_recherche_eqt : IN");
	/*A Recherche de la station RADT concern� */
	for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
	{
		if (
			((!strcmp(pl_ListeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListeRADT->ConfRADT.RGS, va_adrrgs))) ||
			((strcmp(pl_ListeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListeRADT->ConfRADT.AdresseIP, va_adrrgs))))
		{
			/* Sauvegarde donnees radt */
			vl_typeeqt = XDC_EQT_RAD;
			*va_noeqt = pl_ListeRADT->ConfRADT.NumeroStation;
			if (strcmp(pl_ListeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE))
				*va_num_ls = ERDTC_NB_LS - 1;
			else
				*va_num_ls = (pl_ListeRADT->ConfRADT.Liaison - 1);
			XZST_03EcritureTrace(XZSTC_WARNING, "NUM LS %d", *va_num_ls);
			*va_typestation = pl_ListeRADT->ConfRADT.TypeStation;
			*va_horodate = pl_ListeRADT->Don_HisRADT.Horodate_Histo;
			*va_Etat_Eqt = pl_ListeRADT->Etat_Service;
			*va_SiteGestion = pl_ListeRADT->ConfRADT.SiteGestion;
			strcpy(va_adrrgs, pl_ListeRADT->ConfRADT.RGS);
			if (va_typ_rep == XZEXC_TRAME_NON_TRANSM)
				pl_ListeRADT->EtatTrans = ERDTC_EQT_PANNE;
			else
				pl_ListeRADT->EtatTrans = ERDTC_EQT_LIBRE;
			strcpy(vl_station.ConfRADT.RGS, pl_ListeRADT->ConfRADT.RGS);
			strcpy(vl_station.ConfRADT.AdresseIP, pl_ListeRADT->ConfRADT.AdresseIP);
			vl_station.ConfRADT.Port = pl_ListeRADT->ConfRADT.Port;
			vl_station.ConfRADT.DonneesIndividuelles = pl_ListeRADT->ConfRADT.DonneesIndividuelles;
			vl_station.ConfRADT.SiteGestion = pl_ListeRADT->ConfRADT.SiteGestion;
			vl_station.ConfRADT.NumeroStation = pl_ListeRADT->ConfRADT.NumeroStation;
			vl_station.Socket = pl_ListeRADT->Socket;
			vl_station.Don_HisSilhouette = pl_ListeRADT->Don_HisSilhouette;
			vl_station.Don_HisEssieu = pl_ListeRADT->Don_HisEssieu;
			vl_station.Don_HisPoids = pl_ListeRADT->Don_HisPoids;
			vl_station.Don_HisRADT.Horodate_Histo = pl_ListeRADT->Don_HisRADT.Horodate_Histo;
			*pa_station = vl_station;
			XZST_03EcritureTrace(XZSTC_WARNING, "er_recherche_eqt : TROUVE");
			break;
		}
	}

	return (vl_typeeqt);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Recherche de l'etat des voies de la station RADT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_trait_commun(XDY_Mot va_Type_Trame,
					XDY_Eqt va_Num_LS,
					XDY_Eqt va_Num_RADT,
					XDY_Mot va_TypeStation,
					XDY_Horodate va_Horodate,
					XDY_Entier va_Index,
					char *pa_trame,
					XDY_Horodate *va_Horodate_Histo,
					ERDT_EXTRAP *va_Extrapolation)
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Eqt		va_Num_RADT
* XDY_Mot		va_Nb_Minutes
*
* ARGUMENTS EN SORTIE :
*
* XDY_Entier	pa_Index_Tab
* XDY_Entier	pa_Index_Tab_Prec
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*  Recherche de l'etat des voies de la station RADT
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_tra.c,v 1.64 2021/07/12 06:05:03 gesconf Exp $ : er_trait_commun";

	FILE *pl_Fd = NULL;
	char pl_PathEtFic[50] = "";

	/* Variables locales pour les listes des points de mesures et des chaussees */
	ERDT_DONNEES_PtMes *pl_ListePtMes;

	/* Variables locales pour les indices de boucles */
	XDY_Entier vl_i = 0;

	/* Variables locales pour gestion des horodates */

	XZSMT_Horodate vl_heure_string = "";
	XZSMT_Horodate pl_Horo_Txt = "";
	XDY_Horodate vl_Horodate = 0.0;
	XDY_Horodate vl_Heure_Histo = 0.0;
	XDY_Horodate vl_Date_Actuelle = 0.0;
	int vl_Minutes = 0;
	ldiv_t vl_Result_Div;
	//	time_t				vl_time = 0;
	int vl_jourSemaine = 0;
	XZSMT_Horodate pl_Heure_QVTO = "";

	/* Variables locales pour le traitements des mesures */

	XDY_Entier vl_retcode = 0;
	XDY_Entier vl_Nb_Param = 0;

	XDY_Mot vl_Etat_Voie = XDC_VOIE_SENS_NORMAL;

	XDY_Mot vl_Num_Boucle = 0;
	XDY_Mot vl_Nb_Boucles = 0;
	XDY_Mot vl_Sequence = 1;
	XDY_Mot vl_Chaussee = 0;
	int vl_Voie = 0;

	XDY_Entier vl_PL = 0;
	XDY_Entier vl_PL1 = 0;
	XDY_Entier vl_PL1_Inv = 0;
	XDY_Entier vl_PL2 = 0;
	XDY_Entier vl_PL2_Inv = 0;
	XDY_Entier vl_Q = 0;
	XDY_Entier vl_Q_Inv = 0;
	XDY_Entier vl_V = 0;
	XDY_Entier vl_V_Inv = 0;
	XDY_Entier vl_TO = 0;
	XDY_Entier vl_TO_Inv = 0;

	XDY_Entier vl_pas_pointeur = 0;
	XDY_Entier vl_ajt_pointeur = 0;
	XDY_Entier vl_lg_sequence = 0;
	XDY_Entier vl_pas_pointeur_PL = 0;
	XDY_Entier vl_ajt_pointeur_PL = 0;
	XDY_Entier vl_lg_sequence_PL = 0;
	XDY_TrameMesure pl_trame_mesure;
	XDY_TrameMesure pl_trame_mesure_inv;
	XDY_TrameMesure pl_trame_mesure_PL;
	XDY_TrameMesure pl_trame_mesure_PL_inv;
	XDY_Entier vl_longueur = 0;

	XDY_Booleen vl_Donnees_Extrapolees = XDC_FAUX;
	XDY_Booleen vl_Mes_Manquante = XDC_FAUX;

	XZST_03EcritureTrace(XZSTC_FONCTION, " IN : %s RADT = %d\tTYPE=%d\tHORO=%lf\tINDEX=%d", version, va_Num_RADT, va_TypeStation, va_Horodate, va_Index);

	/*A Construction du nom du fichier Erreur_Trame si le fichier erreur_trame existe */
	if (!access("erreur_trame", F_OK))
	{
		sprintf(pl_PathEtFic, "%s/Erreur_Trame_R%d", ERDTC_PATH_FIC_DYN, va_Num_RADT);
		pl_Fd = fopen(pl_PathEtFic, "a+");

		/*A Si la longueur de la trame est inferieur a 15 caracteres alors trace fichier erreur */
		if ((strlen(pa_trame) < 15) && (pl_Fd != NULL))
		{
			XZSM_11ConversionHorodate(va_Horodate, &pl_Horo_Txt);
			fprintf(pl_Fd, "Extrapolation : Hr %s St %d :: Type = %d  Lg = %d  Trame Recue : <%s>\n",
					pl_Horo_Txt, va_Num_RADT, va_Type_Trame, (int)strlen(pa_trame), pa_trame);
		}
	}

	va_Extrapolation->Nb_Extrapolation = 0;
	va_Extrapolation->Chaussee = 0;
	va_Extrapolation->Voie = 0;

	vl_Horodate = va_Horodate;

	if ((va_TypeStation == ERDTC_ECHANGEUR) || (va_TypeStation == ERDTC_PEAGE))
		vl_ajt_pointeur = 5;
	else
		vl_ajt_pointeur = 8;

	vl_ajt_pointeur_PL = 6;

	if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
	{
		/*B Decodage de la trame par voie pour trouver Q, V et TO */

		/*time ( &vl_time );
		 vl_Date_Actuelle = (XDY_Horodate) vl_time;  remplacer par XZSM_07 XZAE_191*/
		XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Date_Actuelle);

		XZSM_11ConversionHorodate(vl_Date_Actuelle, &vl_heure_string);
		vl_Minutes = atoi(&vl_heure_string[ERDTC_DIXMINUTE_STRING]);
		vl_Result_Div = ldiv(vl_Minutes, 6);
		vl_Date_Actuelle -= (vl_Result_Div.rem * 60);
		XZSM_11ConversionHorodate(va_Horodate, &vl_heure_string);
		vl_Minutes = atoi(&vl_heure_string[ERDTC_DIXMINUTE_STRING]);
		vl_Result_Div = ldiv(vl_Minutes, 6);
		vl_Heure_Histo = (va_Horodate < (vl_Date_Actuelle - (719 * 360))) ? (vl_Date_Actuelle - (719 * 360)) : (va_Horodate - (vl_Result_Div.rem * 60));

		/*B Decodage de la trame par voie pour trouver Q, V et TO */
		vl_Num_Boucle = 0;
		if (va_TypeStation == ERDTC_CHAU_ECHA)
		{
			/* sur l'echangeur pas de vitesse uniquement mesures qqqtt */
			vl_lg_sequence = (vl_ajt_pointeur /*!! * 2 */ * pg_Nb_PtMes[va_Num_RADT]) - 6 /*!! 12 */;
			/* sur l'echangeur pas de mesures PL */
			vl_lg_sequence_PL = (vl_ajt_pointeur_PL /*!! * 2 */ * (pg_Nb_PtMes[va_Num_RADT] /*!! - 2 */));
		}
		else
		{
			vl_lg_sequence = vl_ajt_pointeur /*!! * 2 */ * pg_Nb_PtMes[va_Num_RADT];
			vl_lg_sequence_PL = vl_ajt_pointeur_PL /*!! * 2 */ * pg_Nb_PtMes[va_Num_RADT];
		}
	}

	vl_pas_pointeur = 0;

	if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
		vl_longueur = (XDY_Entier)strlen(pa_trame) + 1;
	else
		vl_longueur = (XDY_Entier)strlen(pa_trame);

	/*B Si la trame n'est pas vide => Traitement de celle-ci */
	while ((vl_pas_pointeur < vl_longueur) && (vl_Num_Boucle <= pg_Nb_PtMes[va_Num_RADT]))
	{
		if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
			strcpy(pl_trame_mesure_PL, "");

		strcpy(pl_trame_mesure, "");
		vl_Etat_Voie = XDC_VOIE_INCONNUE;
		vl_Donnees_Extrapolees = XDC_FAUX;
		vl_Mes_Manquante = XDC_FAUX;

		XZST_03EcritureTrace(XZSTC_DEBUG2, "%s : Boucle = %d\tEtat_Voie = %d\tExtrap = %d\tManq = %d\tLong = %d\tPas Ptr = %d\tAjt Ptr = %d",
							 version, vl_Num_Boucle, vl_Etat_Voie, vl_Donnees_Extrapolees, vl_Mes_Manquante, vl_longueur, vl_pas_pointeur, vl_ajt_pointeur);

		if ((vl_Num_Boucle == pg_Nb_PtMes[va_Num_RADT]) && (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS))
		{
			XZST_03EcritureTrace(XZSTC_INFO, "6min His QVTO : ENVOI sequence = %d", vl_Sequence);

			/*B Calcul des valeurs de la chaussee */
			if ((vl_retcode = er_Calcul_Chaussee(va_Num_RADT, va_TypeStation, va_Index, vl_Horodate)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min QVTO : er_Calcul_Chaussee retourne %d", version, vl_retcode);

				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate = vl_Horodate;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
			}
			/*B Stockage des mesures par voie et par chaussee */
			if ((vl_retcode = er_Stockage_Mesures(va_Num_RADT, va_Index, vl_Horodate, XDC_VRAI)) != XDC_OK)
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min His QVTO : er_Stockage_Mesures retourne %d", version, vl_retcode);

			/*B Stockage des mesures par voie et par chaussee */
			if ((vl_retcode = er_Fichier_Mesures(va_Num_RADT, va_Index, vl_Horodate)) != XDC_OK)
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min His QVTO : er_Fichier_Mesures retourne %d", version, vl_retcode);

			if (((vl_pas_pointeur + vl_lg_sequence) > vl_longueur) || (vl_Horodate > vl_Date_Actuelle + 380))
			{
				XZST_03EcritureTrace(XZSTC_DEBUG2, "6min His QVTO : FIN RETURN ");
				vl_pas_pointeur += vl_ajt_pointeur;
				continue;
			}
			vl_Sequence++;
			vl_Num_Boucle = 0;
		}

		/*B Recherche de la chaussee et de l'etat de la voie correspondant a la valeur de la trame envoyee par la station RADT */
		for (pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant)
		{
			/*B Stockage de l'etat des voies de la station RADT dans la variable locale vl_Etat_Voie */
			if ((pl_ListePtMes->ConfPtMes.NumeroStation == va_Num_RADT) &&
				(pl_ListePtMes->ConfPtMes.Boucle == vl_Num_Boucle))
			{
				vl_Etat_Voie = (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS) ? XDC_VOIE_SENS_NORMAL : pl_ListePtMes->EtatVoie;

				/* Indice du tableau correspondant a la chaussee */
				vl_Chaussee = pl_ListePtMes->ConfPtMes.Sens;
				vl_Voie = pl_ListePtMes->ConfPtMes.Voie;
				if ((pl_ListePtMes->ConfPtMes.BoucleInv != vl_Num_Boucle) && (pl_ListePtMes->ConfPtMes.BoucleInv != ERDTC_BOUCL_SS_TRAME))
				{
					vl_Nb_Boucles = 2;
					vl_ajt_pointeur = (va_Type_Trame == XZEXC_TRAME_MES_6MN_PL) ? 6 : 8;
					if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
						vl_ajt_pointeur_PL = 6;
				}
				else
				{
					vl_Nb_Boucles = 1;
					/*vl_ajt_pointeur = ( va_Type_Trame == XZEXC_TRAME_MES_6MN_PL ) ? 6 : 5; */
					vl_ajt_pointeur = (va_Type_Trame == XZEXC_TRAME_MES_6MN_PL) ? 0 : 5;
					if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
						vl_ajt_pointeur_PL = (va_TypeStation == ERDTC_CHAU_ECHA) ? 0 : 6;
				}
				break;
			}
		}

		vl_Num_Boucle++;

		XZST_03EcritureTrace(XZSTC_INFO, "er_trait_commun :station=%d\tSequence = %d\tBoucle = %d/%d\tEtat_Voie = %d\tChaussee = %d\tVoie = %d", va_Num_RADT, vl_Sequence, vl_Num_Boucle, pg_Nb_PtMes[va_Num_RADT], vl_Etat_Voie, vl_Chaussee, vl_Voie);
		XZST_03EcritureTrace(XZSTC_DEBUG1, "er_trait_commun :station=%d\tSequence = %d\tBoucle = %d/%d\tEtat_Voie = %d\tChaussee = %d\tVoie = %d", va_Num_RADT, vl_Sequence, vl_Num_Boucle, pg_Nb_PtMes[va_Num_RADT], vl_Etat_Voie, vl_Chaussee, vl_Voie);

		if (!vg_Mac_Ext_Ouest)
		{
			/*B Selon l'etat de la voie => differents traitements */
			switch (vl_Etat_Voie)
			{
			/*B Cas ou le sens est normal*/
			case XDC_VOIE_SENS_NORMAL:

				/*B  Recuperation des mesures pour le canal correspondant a la boucle de comptage
					 et stockage de ces mesures dans une variable locale pl_trame_mesure */
				strncpy(pl_trame_mesure, pa_trame + vl_pas_pointeur, vl_ajt_pointeur);
				pl_trame_mesure[vl_ajt_pointeur] = '\0';

				if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
				{
					/*B Recuperation des mesures pour le canal correspondant a la boucle de comptage
					et stockage de ces mesures dans une variable locale pl_trame_mesure */
					strncpy(pl_trame_mesure_PL, pg_Trame_PL[va_Num_LS] + vl_pas_pointeur_PL, vl_ajt_pointeur_PL);
					pl_trame_mesure_PL[6] = '\0';
				}
				break;

			/*B Cas ou le sens est inverse */
			case XDC_VOIE_SENS_INVERSE:

				/*B Recuperation des mesures pour le canal correspondant a la boucle de comptage inverse
					et stockage de ces mesures dans une variable locale pl_trame_mesure */
				strncpy(pl_trame_mesure, pa_trame + vl_pas_pointeur + vl_ajt_pointeur, vl_ajt_pointeur);
				pl_trame_mesure[vl_ajt_pointeur] = '\0';
				break;

			/*B Autres cas => on continue de boucler */
			default:

				/* Incrementation du pas du pointeur de pl_trame */
				XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : Continue sur N� Boucle %d ", version, vl_Num_Boucle);
				vl_pas_pointeur += vl_ajt_pointeur;
				if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
					vl_pas_pointeur_PL += vl_ajt_pointeur_PL;
				continue;
			}
		}
		else
		{
			/*B Selon l'etat de la voie => differents traitements */
			switch (vl_Etat_Voie)
			{
			/*B Cas ou le sens est normal*/
			case XDC_VOIE_SENS_NORMAL:
			case XDC_VOIE_SENS_INVERSE:

				/*B  Recuperation des mesures pour le canal correspondant a la boucle de comptage
					 et stockage de ces mesures dans une variable locale pl_trame_mesure */
				strncpy(pl_trame_mesure, pa_trame + vl_pas_pointeur, vl_ajt_pointeur);
				pl_trame_mesure[vl_ajt_pointeur] = '\0';

				/*B Recuperation des mesures pour le canal correspondant a la boucle de comptage inverse
					et stockage de ces mesures dans une variable locale pl_trame_mesure */
				strncpy(pl_trame_mesure_inv, pa_trame + vl_pas_pointeur + vl_ajt_pointeur, vl_ajt_pointeur);
				pl_trame_mesure_inv[vl_ajt_pointeur] = '\0';

				if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
				{
					/*B Recuperation des mesures pour le canal correspondant a la boucle de comptage
					et stockage de ces mesures dans une variable locale pl_trame_mesure */
					strncpy(pl_trame_mesure_PL, pg_Trame_PL[va_Num_LS] + vl_pas_pointeur_PL, vl_ajt_pointeur_PL);
					pl_trame_mesure_PL[6] = '\0';
				}
				break;

			/*B Autres cas => on continue de boucler */
			default:

				/* Incrementation du pas du pointeur de pl_trame */
				XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : Continue sur N� Boucle %d ", version, vl_Num_Boucle);
				vl_pas_pointeur += vl_ajt_pointeur;
				if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
					vl_pas_pointeur_PL += vl_ajt_pointeur_PL;
				continue;
			}
		}

		vl_pas_pointeur += vl_ajt_pointeur;
		if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
			vl_pas_pointeur_PL += vl_ajt_pointeur_PL;

		XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Boucle = %d\tEtat_Voie = %d\tChaussee = %d\tVoie = %d", version, vl_Num_Boucle, vl_Etat_Voie, vl_Chaussee, vl_Voie);
		XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Mesure = <%s>", version, pl_trame_mesure);

		if (va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS)
		{
			for (vl_i = 0; (vl_i < vl_ajt_pointeur) && (vl_ajt_pointeur != 5); vl_i++)
			{
				if ((pl_trame_mesure[vl_i] == ERDTC_BLANC) && (vl_ajt_pointeur != 5))
				{
					vl_Mes_Manquante = XDC_VRAI;
					break;
				}
			}
			/* Horodate des valeurs = derniere horodate des 6min + ( nb_sequences manquantes * 360 ) */
			vl_Horodate = vl_Heure_Histo + (vl_Sequence * 360);

			/*B Si les mesures sont manquantes ou ont deja ete extrapolees
			On continue le traitement jusqu a la fin de la trame des mesures de la station RADT */
			if (vl_Mes_Manquante == XDC_VRAI)
			{
				/*B Affectation des valeurs a manquantes */
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate = vl_Horodate;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
				/*A Si le fichier erreur_trame existe => trace d'erreur trame sur disque */
				if (pl_Fd != NULL)
				{
					XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
					fprintf(pl_Fd, "Recup Histo : Hr %s St %d Ss %d V%d ::\t<%s>\nType = %d  Trame Recue : <%s>\n", pl_Horo_Txt, va_Num_RADT, vl_Chaussee, vl_Voie,
							pl_trame_mesure, va_Type_Trame, pa_trame);
				}
				continue;
			}
		}
		else
		{
			/*B Si la trame est incoherente => Extrapolation */
			if (vl_ajt_pointeur != 5)
			{
				for (vl_i = 0; vl_i < vl_ajt_pointeur; vl_i++)
				{
					if ((pl_trame_mesure[vl_i] < '0') || (pl_trame_mesure[vl_i] > '9'))
					{
						XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Reponse Incorrecte => Extrapolation", version);
						/*A Si le fichier erreur_trame existe => trace d'erreur trame sur disque */
						if (pl_Fd != NULL)
						{
							XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
							fprintf(pl_Fd, "Extrapolation : Hr %s St %d Ss %d V%d ::\t<%s>\nType = %d  Trame Recue : <%s>\n", pl_Horo_Txt, va_Num_RADT,
									vl_Chaussee, vl_Voie, pl_trame_mesure, va_Type_Trame, pa_trame);
						}
						vl_Donnees_Extrapolees = XDC_VRAI;
						break;
					}
				}
			}
			/*B Si la variable vl_Donnees_Extrapolees est positionnee => continue */
			if (vl_Donnees_Extrapolees == XDC_VRAI)
			{
				if (strlen(pa_trame) < 15)
				{
					/*A Si le fichier erreur_trame existe => trace d'erreur trame sur disque */
					if (pl_Fd != NULL)
					{
						XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
						fprintf(pl_Fd, "Extrapolation : Hr %s St %d Ss %d V%d ::\t<%s>\nType = %d  Trame Recue : <%s>\n", pl_Horo_Txt, va_Num_RADT,
								vl_Chaussee, vl_Voie, pl_trame_mesure, va_Type_Trame, pa_trame);
					}
					va_Extrapolation->Nb_Extrapolation += 2;
				}
				else
					va_Extrapolation->Nb_Extrapolation++;

				if (va_Extrapolation->Nb_Extrapolation >= 2)
				{
					XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : %s : HORO = %lf\tNB EXT = %d\tCHAUS = %d\tVOIE = %d", version, *va_Horodate_Histo,
										 va_Extrapolation->Nb_Extrapolation, va_Extrapolation->Chaussee, va_Extrapolation->Voie);
					if (pl_Fd != NULL)
						fclose(pl_Fd);
					return (XDC_OK);
				}
				va_Extrapolation->Chaussee = vl_Chaussee;
				va_Extrapolation->Voie = vl_Voie;
				continue;
			}
		}

		/*B Si les mesures sont correctes => Remplissage de la structure du tableau pg_ERDT_Mes_1mn pour l'index en cours
			Separation des mesures Q, TO et V de pl_trame_mesure et Conversion de ces mesures, recuperees en string, en valeurs numeriques */

		if (va_Type_Trame != XZEXC_TRAME_MES_6MN_PL)
		{
			/*B Si la chaussee est de sens sud ou nord => recuperation de Q, V et TO */
			if (vl_Nb_Boucles == 2)
			{
				if (!vg_Mac_Ext_Ouest)
					sscanf(pl_trame_mesure, "%3d%2d%3d", &vl_Q, &vl_TO, &vl_V);
				else
				{
					sscanf(pl_trame_mesure, "%3d%2d%3d", &vl_Q, &vl_TO, &vl_V);
					sscanf(pl_trame_mesure_inv, "%3d%2d%3d", &vl_Q_Inv, &vl_TO_Inv, &vl_V_Inv);
					vl_Q = (vl_Q > vl_Q_Inv) ? vl_Q : vl_Q_Inv;
					vl_V = (vl_Q > vl_Q_Inv) ? vl_V : vl_V_Inv;
					vl_TO = (vl_Q > vl_Q_Inv) ? vl_TO : vl_TO_Inv;
				}
			}
			else
			{
				/*B Si la chaussee est une bretelle d'entree ou de sortie echangeur => recuperation de Q et TO */
				if (pl_trame_mesure[3] != ERDTC_BLANC)
				{
					vl_V = XDC_RADT_V_INCONNU;
					sscanf(pl_trame_mesure, "%3d%2d", &vl_Q, &vl_TO);
				}
				/*B Si la chaussee est une gare de peage => recuperation de Q */
				else
				{
					vl_V = XDC_RADT_V_INCONNU;
					vl_TO = XDC_RADT_TO_INCONNU;
					sscanf(pl_trame_mesure, "%3d", &vl_Q);
				}
			} /* fin else vl_Nb_Boucles */
		}

		if ((va_Type_Trame == XZEXC_TRAME_MES_6MN_PL) ||
			((va_Type_Trame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS) && (vg_Rep_His_PL[va_Num_LS] == XDC_VRAI)))
		{
			/*B Separation des mesures PL de pl_trame_mesure_PL
			Conversion de ces mesures, recuperees en string, en valeurs numeriques */
			if (va_Type_Trame == XZEXC_TRAME_MES_6MN_PL)
				strcpy(pl_trame_mesure_PL, pl_trame_mesure);
			if ((va_Type_Trame == XZEXC_TRAME_MES_6MN_PL) &&
				(vg_Mac_Ext_Ouest))
				strcpy(pl_trame_mesure_PL_inv, pl_trame_mesure_inv);
			if (strlen(pl_trame_mesure_PL) == 6)
			{
				sscanf(pl_trame_mesure_PL, "%3d%3d", &vl_PL1, &vl_PL2);
				if (vg_Mac_Ext_Ouest)
				{
					sscanf(pl_trame_mesure_PL_inv, "%3d%3d", &vl_PL1_Inv, &vl_PL2_Inv);
					vl_PL1 = (vl_PL1 > vl_PL1_Inv) ? vl_PL1 : vl_PL1_Inv;
					vl_PL2 = (vl_PL1 > vl_PL1_Inv) ? vl_PL2 : vl_PL2_Inv;
				}
				XZST_03EcritureTrace(XZSTC_DEBUG3, "6min PL : PL1 = %d\tPL2 = %d", vl_PL1, vl_PL2);

				if ((vl_PL1 + vl_PL2) > 0)
					vl_PL = (((float)vl_PL2 / (float)(vl_PL1 + vl_PL2))) * 100;
				else if (vl_PL2 == 0)
					vl_PL = vl_PL2;
				else
					vl_PL = XDC_RADT_PL_INCONNU;
				if (vl_Nb_Boucles == 1)
					vl_PL = XDC_RADT_PL_INCONNU;
			}
			else
				vl_PL = XDC_RADT_PL_INCONNU;
		}

		XZST_03EcritureTrace(XZSTC_DEBUG3, "Recup. val. trame: Nb Boucles = %d\tQ = %d\tV = %d\tTO = %d\tPL= %d",
							 vl_Nb_Boucles, vl_Q, vl_V, vl_TO, vl_PL);

		XZST_03EcritureTrace(XZSTC_INFO, "Recup. val. trame: Nb Boucles = %d\tQ = %d\tV = %d\tTO = %d\tPL= %d",
							 vl_Nb_Boucles, vl_Q, vl_V, vl_TO, vl_PL);
		switch (va_Type_Trame)
		{
		case XZEXC_TRAME_MES_1MN_Q_V_TO:
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate = vl_Horodate;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val = (vl_Q != XDC_RADT_Q_INCONNU) ? (60 * vl_Q) : XDC_RADT_Q_INCONNU;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic = (vl_Q != XDC_RADT_Q_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val = vl_TO;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic = (vl_TO != XDC_RADT_TO_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val = vl_V;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic = (vl_V != XDC_RADT_V_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
			/* Trace dans un fichier erreur trame si celui ci existe */
			if ((((vl_Q < 0) && (vl_TO == XDC_RADT_TO_INCONNU) && (vl_V == XDC_RADT_V_INCONNU)) || (vl_Q > 2000)) && (pl_Fd != NULL))
			{
				XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
				fprintf(pl_Fd, "1 Mn : Hr %s St %d Ss %d V%d ::\t<%s>\nTrame Recue : <%s>\n", pl_Horo_Txt, va_Num_RADT, vl_Chaussee, vl_Voie, pl_trame_mesure, pa_trame);
			}
			break;

		case XZEXC_TRAME_MES_6MN_PL:
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate = vl_Horodate;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val = vl_PL;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic = (vl_PL != XDC_RADT_PL_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
			for (vl_i = 0; vl_i < ERDTC_NB_INDEX_1MN; vl_i++)
			{
				vg_PL_Prec[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie] = pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_i].Mesures.ValPL.Val;
				vg_PL_Prec[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0] = pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][vl_i].Mesures.ValPL.Val;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_i].Horodate = vl_Horodate;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][vl_i].Mesures.ValPL.Val = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][vl_i].Mesures.ValPL.Indic = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_i].Mesures.ValPL.Val = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_i].Mesures.ValPL.Indic = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic;
			}
			/* Trace dans un fichier erreur trame */
			if ((vl_PL == XDC_RADT_PL_INCONNU) && (pl_Fd != NULL))
			{
				XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
				fprintf(pl_Fd, "6 Mn PL : Hr %s St %d Ss %d V%d ::\t<%s>\nTrame Recue : <%s>\n", pl_Horo_Txt, va_Num_RADT, vl_Chaussee, vl_Voie, pl_trame_mesure, pa_trame);
			}

			break;

		case XZEXC_TRAME_MES_6MN_Q_V_TO:
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate = vl_Horodate;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val = vl_Q * 10;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic = XDC_RADT_CORRECT;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val = vl_TO;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic = (vl_TO != XDC_RADT_TO_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val = vl_V;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic = (vl_V != XDC_RADT_V_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
			/* Trace dans un fichier erreur trame */
			if ((((vl_Q < 0) && (vl_TO == XDC_RADT_TO_INCONNU) && (vl_V == XDC_RADT_V_INCONNU)) || (vl_Q > 2000)) && (pl_Fd != NULL))
			{
				XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
				fprintf(pl_Fd, "6 Mn QVTO : Hr %s St %d Ss %d V%d ::\t<%s>\nTrame Recue : <%s>\n", pl_Horo_Txt, va_Num_RADT, vl_Chaussee, vl_Voie, pl_trame_mesure, pa_trame);
			}
			break;

		case XZEXC_TRAME_MES_6MN_Q_V_TO_HIS:
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate = vl_Horodate;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val = vl_Q * 10;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic = XDC_RADT_CORRECT;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val = vl_TO;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic = (vl_TO != XDC_RADT_TO_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val = vl_V;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic = (vl_V != XDC_RADT_V_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val = vl_PL;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic = (vl_PL != XDC_RADT_PL_INCONNU) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;

			/* Trace dans un fichier erreur trame */
			if (((vl_Q < 0) && (vl_TO == XDC_RADT_TO_INCONNU) && (vl_V == XDC_RADT_V_INCONNU)) && (pl_Fd != NULL))
			{
				XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
				fprintf(pl_Fd, "6 Mn HIS : Hr %s St %d Ss %d V%d ::\t<%s>\nTrame Recue : <%s>\n", pl_Horo_Txt, va_Num_RADT, vl_Chaussee, vl_Voie, pl_trame_mesure, pa_trame);
			}

			break;

		} /* fin switch va_Type_Trame */

		XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Fin Boucle Une valeur traitee", version);

		XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Pas Ptr = %d\tBoucle = %d", version, vl_pas_pointeur, vl_Num_Boucle);
	} /* Fin while */

	XZSM_11ConversionHorodate(vl_Horodate, &vl_heure_string);
	XZST_03EcritureTrace(XZSTC_INFO, "%s HORO SORTIE = %s", version, vl_heure_string);
	*va_Horodate_Histo = vl_Horodate;

	XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : %s : HORO = %lf\tNB EXT = %d\tCHAUS = %d\tVOIE = %d", version, *va_Horodate_Histo,
						 va_Extrapolation->Nb_Extrapolation, va_Extrapolation->Chaussee, va_Extrapolation->Voie);

	if (pl_Fd != NULL)
		fclose(pl_Fd);
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

int er_traitement_trame(int va_socket)

/*
* ARGUMENTS EN ENTREE :
*
* va_socket : no socket sur laquelle le message est arriv�
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
	static char *version = "$Id: erdt_tra.c,v 1.64 2021/07/12 06:05:03 gesconf Exp $ : er_traitement_trame";

	int vl_IndCnx = 0;
	int vl_LgMsg = 0;
	char *pl_trame_ptr;
	char vl_entete1[10];
	char vl_entete2[10];
	char vl_adrrgs[20];
	XDY_Entier vl_typetrame = 0;
	XDY_Entier vl_typ_rep = 0;
	XZEXT_MSG_SOCKET vl_Msg;
	XDY_Entier vl_param = 0;
	XDY_Entier vl_nbnonrep = 0;
	XDY_Entier vl_nberrcrc = 0;
	XDY_Horodate vl_horodate = 0;
	int vl_nocmd = 0;
	char vl_horod1[20];
	char vl_horod2[9];
	XZSMT_Horodate pl_Horodatedecompo;
	XDY_Entier vl_typeeqt = 0;
	XZEXT_TEXTE_STATUS vl_tabstatus[11];

	/* Variables locales pour les listes des stations, des points de mesures et des chaussees */
	ERDT_DONNEES_PtMes *pl_ListePtMes;
	ERDT_DONNEES_Chaussee *pl_ListeChaussee;

	/* Variables locales pour la trame recue */
	char pl_trame[ERDTC_DIM_TRAME] = "";

	/* Variables locales pour les indices de boucles */
	XDY_Entier vl_i = 0;

	/* Variables locales pour les Station de comptage */
	XDY_Mot vl_Num_RADT = 0;
	XDY_Mot vl_Num_LS = 0;
	XDY_Mot vl_TypeStation = 0;
	XDY_Entier vl_EtatEqt = 0;

	/* Variables locales pour gestion des fichiers */

	XDY_Texte pl_PathEtFic = "";
	XDY_Texte pl_PathFicErr = "";
	FILE *pl_Fd;
	FILE *pl_Fd_Stock;

	/* Variables locales pour gestion des horodates */

	XZSMT_Horodate vl_heure_string = "";
	XZSMT_Horodate pl_Horo_Txt = "";
	//	time_t				vl_time = 0;
	int vl_jourSemaine = 0;
	XDY_Horodate vl_horodateSec = 0.0;
	XDY_Horodate vl_Horodate = 0.0;
	XDY_Horodate vl_Horodate_Histo = 0.0;
	XDY_Horodate vl_Heure_Histo = 0.0;
	XDY_Horodate vl_Horodate_Prec = 0.0;
	int vl_heures = 0;
	int vl_minutes = 0;
	int vl_secondes = 0;
	int vl_Minutes = 0;

	/* Variables locales pour les index des tableaux */
	XDY_Entier vl_Index = 0;
	XDY_Entier vl_Index_Prec = 0;

	/* Variables locales pour le traitements des mesures */

	XDY_Entier vl_retcode = 0;
	XDY_Entier vl_Nb_Param = 0;

	XDY_Booleen vl_Continue = XDC_FAUX;
	XDY_Booleen vl_Mesures_Individuelles = XDC_FAUX;

	XDY_Mot vl_Etat_Voie = XDC_VOIE_SENS_NORMAL;
	XDY_Booleen vl_Capteur_Poids = XDC_FAUX;

	char vl_Num_Boucle_str = ' ';
	XDY_Mot vl_Num_Boucle = 0;
	XDY_Mot vl_Chaussee = 0;
	int vl_Voie = 0;
	int vl_Sens = 0;

	XDY_Entier vl_pas_pointeur = 0;
	XDY_Entier vl_ajt_pointeur = 0;
	XDY_TrameMesure pl_trame_mesure = "";
	char pl_trame_status = ' ';
	XDY_Entier vl_longueur = 0;

	ERDT_EXTRAP vl_Extrapolation;
	XDY_Booleen vl_Mes_Extrapolees = XDC_FAUX;
	XDY_Booleen vl_Mes_Manquante = XDC_FAUX;

	/* Variables pour demande histo MB */

	XDY_Entier vl_Nb_Val_PL_Demand = 1;
	XDY_Entier vl_Nb_Val_QVTO_Demand = 1;
	char pl_Demand_His_PL[9];
	char pl_Demand_His_QVTO[9];
	ldiv_t vl_Result_Div;

	/* Variables pour le calcul des DIV */

	XDY_Entier vl_Q = 0;
	XDY_Entier vl_V = 0;
	int vl_SensVoie = 0;
	int vl_Num_Voie = 0;
	int vl_Long_metre = 0;
	int vl_Long_decim = 0;
	int vl_Sec = 0;
	int vl_mSec = 0;
	int vl_Vit = 0;
	double vl_DIV = 0;
	static double vl_TempInterVeh = 0;
	static float vl_SecondePassage = 0;
	int vl_Silhouette = 0;
	XDY_Entier vl_Poids = 0;
	int vl_Nb_Essieu = 0;
	ERDT_DONNEES_RADT vl_station;
	/* Variables locales pour envoi des mesures a TETDC */

	XZEXT_MSG_SOCKET pl_MsgTDC = "";
	int vl_Nb_Chaussees = 0;
	int vl_Flag_Envoi = 0;
	int vl_Cpt_Flag = 0;
	XZSCT_NomTache vl_NomTache = "";
	XDY_NomMachine vl_NomMachine = "";
	ERDT_MESURE_PESAGE vl_Pesage;
	ERDT_DON_HIS_RADT *pl_HistoPesage = NULL;

	XDY_District vl_SiteGestion;

	/*si simu*/
	if (vg_mode_simu)
	{
		return (XDC_OK);
	}
	/*A  Lecture du message recu dans la socket appelante */
	if ((vl_retcode = ex_lire_msg(va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "er_traitement_trame : Erreur lecture message socket. ");
	}
	else
	{
		/*A S'il s'agit d'une deconnexion */
		if (vl_IndCnx == XZEXC_DECONNEC)
		{
			/*B Arreter la tache par appel de er_sortir */
			XZST_03EcritureTrace(XZSTC_WARNING, "XZEXC_DECONNEC er_sortir");
			er_sortir();
		}

		/*A  Rangement du message provenant de la socket dans des variables locales */

		vl_param = sscanf(vl_Msg, "%s %s %s %s %d %d %d %s %d %d", vl_entete1,
						  vl_entete2,
						  vl_horod1,
						  vl_horod2,
						  &vl_nbnonrep,
						  &vl_nberrcrc,
						  &vl_nocmd,
						  vl_adrrgs,
						  &vl_typetrame,
						  &vl_typ_rep);
		if (vl_param != 10)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "er_traitement_trame : Lecture message socket impossible mes=%s", vl_Msg);
			return (XDC_NOK);
		}
		else
			XZST_03EcritureTrace(XZSTC_WARNING, "TRAME %s", vl_Msg);

		/*A Recherche de l'horodate systeme */
		/*time ( &vl_time );
		vl_horodateSec = (XDY_Horodate) vl_time;  remplacer par XZSM_07 XZAE_191*/
		XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_horodateSec);
		/*A Conversion de l'horodate recue */
		sscanf(vl_horod2, "%02d:%02d:", &vl_heures, &vl_minutes);
		sprintf(pl_Horodatedecompo, "%s %02d:%02d:%s", vl_horod1, vl_heures, vl_minutes, "01");
		XZSM_13ConversionHorodateSec(pl_Horodatedecompo, &vl_horodate);
		vl_Horodate = (vl_horodate < (vl_horodateSec - 3600)) ? vl_horodateSec : vl_horodate;

		/*A Recherche l'equipement qui emet une reponse */
		XZST_03EcritureTrace(XZSTC_WARNING, "%s ", vl_adrrgs);
		if ((vl_typeeqt = er_recherche_eqt(vl_adrrgs, vl_typ_rep, &vl_Num_RADT, &vl_Num_LS, &vl_TypeStation, &vl_Heure_Histo, &vl_EtatEqt, &vl_SiteGestion, &vl_station)) == 0)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "%s : RADT Non trouve en memoire mes=%s", version, vl_Msg);
			return (XDC_NOK);
		}

		XZST_03EcritureTrace(XZSTC_INFO, "DEM476: fonction er_traitement_trame pour station= %d ", vl_Num_RADT);
		XZST_03EcritureTrace(XZSTC_INFO, "DEM476: ADRIP %s %d",
							 vl_station.ConfRADT.AdresseIP, vl_station.ConfRADT.Port);
		/*A Traitement des alarmes transmission avec l'equipement */
		if (!(vl_EtatEqt & XDC_EQT_HORS_SRV) && (vl_typetrame != XZEXC_TRAME_FEN_LCR))
			ex_traite_alarme_com(vg_NomMachine,
								 vl_typeeqt,
								 vl_Num_RADT,
								 vl_typ_rep,
								 vl_nbnonrep,
								 vl_nberrcrc,
								 vl_typetrame,
								 vg_Mode_Fonct,
								 vl_SiteGestion);

		/*A On se positionne sur le debut de la trame */
		pl_trame_ptr = strstr(vl_Msg, XZEXC_SEP_TRAME);
		pl_trame_ptr += strlen(XZEXC_SEP_TRAME);
		while (*pl_trame_ptr == ERDTC_BLANC)
		{
			pl_trame_ptr += 1;
		}
		/*B Ignorer les premiers caracteres pour certains types de trames si besoin */
		/*if ( (vl_typetrame == XZEXC_TRAME_MES_6MN_Q_V_TO_HIS) || (vl_typetrame == XZEXC_TRAME_MES_6MN_PL_HIS) )
		{
		   pl_trame_ptr += 1;
		}*/

		/*A Si le bloc est intermediaire, on le stocke */
		/* Suppression de la fin de trame (fin message socket) */
		strtok(pl_trame_ptr, XZEXC_FIN_MSG_SOCK);
		vl_longueur = (XDY_Entier)strlen(pl_trame_ptr);

		if (vl_typetrame != XZEXC_TRAME_FEN_LCR)
		{
			if (vl_typ_rep == XZEXC_REP_BLC_INT)
			{
				strncat(pg_Trame[vl_Num_LS], pl_trame_ptr, ((size_t)vl_longueur - 1));
				return (XDC_OK);
			}
			else
			{
				strncat(pg_Trame[vl_Num_LS], pl_trame_ptr, vl_longueur);
				strncpy(pl_trame, pg_Trame[vl_Num_LS], strlen(pg_Trame[vl_Num_LS]));
				strcpy(pg_Trame[vl_Num_LS], "");
			}
		}

		/*A Si la lecture de la trame est impossible => Sortie du traitement */
		if (pl_trame == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, " Lecture message socket impossible mes=%s", vl_Msg);
			return (XDC_NOK);
		}

		vl_longueur = (XDY_Entier)strlen(pl_trame);
		XZST_03EcritureTrace(XZSTC_INFO, "ADRIP %d", vl_typetrame);

		/*A Recherche du type de trame */
		switch (vl_typetrame)
		{
		/*A ******** C'est une trame de reponse pour la fenetre LCR ******** */
		case XZEXC_TRAME_FEN_LCR:

			XZST_03EcritureTrace(XZSTC_INFO, "er_traitement_trame : Cas Fenetre LCR ");

			/*A Appel de ex_retlcr */
			ex_ret_LCR(pl_trame_ptr, vl_adrrgs, &pg_DebutListeRADTFLcr, vl_typ_rep);

			XZST_03EcritureTrace(XZSTC_DEBUG1, "rgs:%s trame:%s ", vl_adrrgs, pl_trame);
			break;

		/*A ******** C'est une trame de reponse status temps reel ******** */
		case XZEXC_TRAME_STAT_TR:

			XZST_03EcritureTrace(XZSTC_INFO, "er_traitement_trame : Cas Status TR ");

			/*A Si le status TR est ko -> demande du status complet */
			if (strcmp(pl_trame_ptr, "@"))
			{
				if (!strcmp(vl_station.ConfRADT.AdresseIP, XZECC_IP_VIDE))
					er_EnvoyerTrame("ST",
									XZEXC_TRAME_STAT_COMP,
									vl_adrrgs,
									XZEXC_NOCMD,
									XZEXC_PRIORITEFAIBLE,
									va_socket);
				else
					EnvoyerTrameIP("ST",
								   XZEXC_TRAME_STAT_COMP,
								   vl_station.ConfRADT.AdresseIP,
								   vl_station.ConfRADT.Port,
								   vl_station.ConfRADT.RGS,
								   XZEXC_NOCMD,
								   XZEXC_PRIORITEFAIBLE,
								   vl_station.Socket);
			}
			break;

		/*A ******** c'est une trame de status complet (retour ST) ******** */
		case XZEXC_TRAME_STAT_COMP:

			XZST_03EcritureTrace(XZSTC_INFO, "er_traitement_trame : Demande Status Complet %d",
								 vl_station.ConfRADT.NumeroStation);

			if ((vl_typ_rep != XZEXC_TRAME_NON_TRANSM) && (vl_typ_rep != XZEXC_REP_NAK))
			{

				if (strlen(vl_adrrgs) < 3)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : Cas Status COMP :  RGS = %d station = %s incorrecte", vl_adrrgs, vl_Num_RADT);
					break;
				}
				XZST_03EcritureTrace(XZSTC_INFO, "%s : Cas Status COMP : tab = %s ## rgs = %s ## horo = %lf ## type = %d ## num = %d ## mode =%d ## mac =%s",
									 version, vl_tabstatus[1], vl_adrrgs, vl_horodate, XDC_EQT_RAD,
									 vl_station.ConfRADT.NumeroStation, vg_Mode_Fonct, vg_NomMachine);
				/*A Decodage de la trame de status */
				ex_decode_status(pl_trame,
								 vl_tabstatus,
								 10,
								 vl_adrrgs,
								 vl_horodate,
								 XDC_EQT_RAD,
								 vl_station.ConfRADT.NumeroStation,
								 vg_NomMachine,
								 vl_station.ConfRADT.SiteGestion);

				/* Mise jour compteur de marche equipement sur defaut EDF */
				XZSC_06NomTache(vl_NomTache);
				XZSC_07NomMachine(vl_NomMachine);
				if (atoi(vl_tabstatus[2]) == 0)
				{
					XZMA01_Maj_duree_marche(vl_station.ConfRADT.NumeroStation,
											XDC_EQT_RAD,
											XDC_FAUX,
											vl_NomTache,
											vl_NomMachine);
				}
				else
				{
					XZMA01_Maj_duree_marche(vl_station.ConfRADT.NumeroStation,
											XDC_EQT_RAD,
											XDC_VRAI,
											vl_NomTache,
											vl_NomMachine);
				}

				/*!!!!!*/
				if ((vl_TypeStation != ERDTC_PEAGE) && (vl_Num_RADT != 24))
				{
					ex_traitBAT(vl_tabstatus[8], vl_horodate, XDC_EQT_RAD, vl_station.ConfRADT.NumeroStation, vg_NomMachine,
								vl_station.ConfRADT.SiteGestion);
					if (atoi(vl_tabstatus[8]) == 0)
					{
						ex_traitBTR(vl_tabstatus[9], vl_horodate, XDC_EQT_RAD, vl_station.ConfRADT.NumeroStation, vg_NomMachine,
									vl_station.ConfRADT.SiteGestion);
						XZST_03EcritureTrace(XZSTC_INFO, "%s : Cas Status COMP : BAT = %s :: BTR = <%s>", version, vl_tabstatus[8], vl_tabstatus[9]);
					}
					else
					{
						ex_traitBTR("0", vl_horodate, XDC_EQT_RAD, vl_station.ConfRADT.NumeroStation, vg_NomMachine,
									vl_station.ConfRADT.SiteGestion);
						XZST_03EcritureTrace(XZSTC_INFO, "%s : Cas Status COMP : BAT = %s :: BTR = <%s>", version, vl_tabstatus[8], vl_tabstatus[9]);
					}
				}
			}
			break;

			/*A ******** Cas des mesures 1mn Q V TO ******** */
		case XZEXC_TRAME_MES_1MN_Q_V_TO:

			XZST_03EcritureTrace(XZSTC_INFO, "er_traitement_trame : Cas des mesures 1mn Q V TO : St = %d Hr = %s trame = <%s>", vl_Num_RADT, pl_Horodatedecompo, pl_trame);

			if ((strstr(pl_trame, ERDTC_STATUS_TMP_REEL_STRG) == NULL) &&
				(vl_typ_rep != XZEXC_TRAME_NON_TRANSM) &&
				(vl_typ_rep != XZEXC_REP_NAK))
			{
				if (!strcmp(vl_station.ConfRADT.AdresseIP, XZECC_IP_VIDE))
					er_EnvoyerTrame("ST",
									XZEXC_TRAME_STAT_COMP,
									vl_adrrgs,
									XZEXC_NOCMD,
									XZEXC_PRIORITEFAIBLE,
									va_socket);
				else
					EnvoyerTrameIP("ST",
								   XZEXC_TRAME_STAT_COMP,
								   vl_station.ConfRADT.AdresseIP,
								   vl_station.ConfRADT.Port,
								   vl_station.ConfRADT.RGS,
								   XZEXC_NOCMD,
								   XZEXC_PRIORITEFAIBLE,
								   vl_station.Socket);
				strncpy(pl_trame, pl_trame, strlen(pl_trame) - 1);
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : Manque @", version);
			}
			else
				strtok(pl_trame, ERDTC_STATUS_TMP_REEL_STRG);

			/*B Recherche de l'index de stockage dans le tableau des Mesures 1mn */
			if ((vl_retcode = er_recherche_index(vl_Num_RADT, vl_minutes, ERDTC_MINUTE, &vl_Index, &vl_Index_Prec)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_recherche_index retourne %d", version, vl_retcode);
				return (XDC_OK);
			}

			/*B JPL Initialisation de toutes les donnees de cette station a cette date (cet index) dans tous les cas */
			for (vl_Sens = 1; vl_Sens < ERDTC_NB_CHAUSSEE; (vl_Sens == 4) ? vl_Sens += 3 : vl_Sens++)
				for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Horodate = vl_Horodate;
					pg_ERDT_Mes_1mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures = vg_Mesures_Aucune;
				}

			/*B Lecture de la trame et stockage des valeurs tant que la fin de trame n'est pas atteinte */
			XZST_03EcritureTrace(XZSTC_DEBUG2, "traitement : Trame = <%s>", pl_trame);

			/*B Si Non Reponse de la station */
			if ((vl_typ_rep == XZEXC_TRAME_NON_TRANSM) || (vl_typ_rep == XZEXC_REP_NAK))
			{
				XZST_03EcritureTrace(XZSTC_INFO, "%s : 1mn Q V TO : aucune reponse ", version);

				/*DEM476 Mise en commentaire de la fonction d'extrapolation */
				/*vl_Mes_Extrapolees = XDC_VRAI;
				if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, ERDTC_TOUS_SENS, ERDTC_TOUTES_VOIES, ERDTC_MINUTE, ERDTC_QVTO_PL, vl_TypeStation ) ) != XDC_OK )
				{
				   XZST_03EcritureTrace( XZSTC_WARNING, "%s : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
				}*/

				/* Construction du nom du fichier Erreur_Trame si le fichier erreur_trame existe */
				if (!access("erreur_trame", F_OK))
				{
					sprintf(pl_PathFicErr, "%s/Erreur_Trame_R%d", ERDTC_PATH_FIC_DYN, vl_Num_RADT);
					pl_Fd = fopen(pl_PathFicErr, "a+");
					XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
					fprintf(pl_Fd, "1 Mn : Hr %s St %d :: NON REPONSE\n", pl_Horo_Txt, vl_Num_RADT);
					fclose(pl_Fd);
				}
			}
			/*B Sinon Si Reponse de la station */
			else
			{
				/*B Appel de la fonction de traitement d'un trame => er_trait_commun */
				if ((vl_retcode = er_trait_commun(XZEXC_TRAME_MES_1MN_Q_V_TO, vl_Num_LS, vl_Num_RADT, vl_TypeStation, vl_Horodate, vl_Index,
												  pl_trame, &vl_Horodate, &vl_Extrapolation)) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_trait_commun retourne : %d", version, vl_retcode);
				}

				/*DEM476 Mise en commentaire de la fonction d'extrapolation */
				/*A Si une seule voie est manquante => Extrapolation en fonction des autres voies de la station */
				/*if ( vl_Extrapolation.Nb_Extrapolation == 1 )
				{
				   XZST_03EcritureTrace ( XZSTC_INFO, "%s : appel fonction er_Extrapolation", version );

				   if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, vl_Extrapolation.Chaussee, vl_Extrapolation.Voie, ERDTC_MINUTE, ERDTC_QVTO_PL, vl_TypeStation ) ) != XDC_OK )
				   {
					  XZST_03EcritureTrace( XZSTC_WARNING, "%s : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
				   }
				 }*/
				/*A Si Plus d'une voie est manquantes => Extrapolation en fonction des stations amont et aval */
				/*else
				{
				   if ( vl_Extrapolation.Nb_Extrapolation > 1 )
				   {
					  XZST_03EcritureTrace ( XZSTC_INFO, "%s : appel fonction er_Extrapolation", version );
				  vl_Mes_Extrapolees = XDC_VRAI;
					  if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, ERDTC_TOUS_SENS, ERDTC_TOUTES_VOIES, ERDTC_MINUTE, ERDTC_QVTO_PL, vl_TypeStation ) ) != XDC_OK )
					  {
					 XZST_03EcritureTrace( XZSTC_WARNING, "%s : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
				  }
				   }
			   }*/
			} /* Fin Sinon Si Reponse de la station */

			/*A Calcul et envoi des mesures glissantes => Appel de la fonction er_Mesures_Glissantes du module MCAL */
			XZST_03EcritureTrace(XZSTC_INFO, "%s : appel fonction er_Mesures_Glissantes", version);

			if ((vl_retcode = er_Mesures_Glissantes(vl_Horodate, vl_Num_RADT, vl_Index, vl_Mes_Extrapolees)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "er_traitement_trame : er_Mesures_Glissantes retourne : %d", vl_retcode);
			}

			/*A Calcul et envoi des niveaux de services => Appel de la fonction er_Niveau_Service du module MCAL */
			XZST_03EcritureTrace(XZSTC_DEBUG2, "%s : appel fonction er_Niveau_Service", version);

			if ((vl_retcode = er_Niveau_Service(vl_Num_RADT, vl_TypeStation, vl_Index, vl_Horodate, ERDTC_MINUTE)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_Niveau_Service retourne : %d", version, vl_retcode);
			}

			/*A Si l'acquisition des donnees individuelles a lieu */
			if (vl_station.ConfRADT.DonneesIndividuelles == XDC_VRAI)
			{
				XZST_03EcritureTrace(XZSTC_DEBUG2, "%s : Calcul de la DIV Moyenne pour sens sud et nord", version);
				/*A Calcul de la DIV moyenne */
				for (vl_i = 0; vl_i < ERDTC_NB_DIV_MOY; vl_i++)
				{
					if ((pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_i + 1][0][vl_Index].Voie != XDC_VOIE_INCONNUE) &&
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_i + 1][0][vl_Index].Mesures.ValV.Val != XDC_RADT_V_INCONNU) &&
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_i + 1][0][vl_Index].Mesures.ValQ.Val != XDC_RADT_Q_INCONNU))
					{
						vl_V = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_i + 1][0][vl_Index].Mesures.ValV.Val;
						vl_Q = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][vl_i + 1][0][vl_Index].Mesures.ValQ.Val;

						if ((vl_Q - vl_V) > 0)
							pg_DIV_Moy[vl_i] = (double)(1000 * vl_V - 5 * vl_Q) / (vl_Q - vl_V);
						XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : DIV Moyenne = %d\tV = %d\tQ = %d", version, pg_DIV_Moy[vl_i], vl_V, vl_Q);
					}
				}
			}

			/*A Envoi de la trame des mesures recues � TETDC pour calcul des seuils d'alertes */
			if (!(vl_EtatEqt) && !vg_Mac_Ext_Ouest)
			{
				/* Mise a jour du flag de premier ou dernier envoi */
				XZST_03EcritureTrace(XZSTC_INFO, "%s : Envoi Message a TETDC pour Calcul Equation d'Alerte", version);
				vl_Nb_Chaussees = pg_Nb_Chaussees[vl_Num_RADT] - 1;
				for (pl_ListeChaussee = pg_debutChaussee; pl_ListeChaussee != NULL; pl_ListeChaussee = pl_ListeChaussee->Suivant)
				{
					if (pl_ListeChaussee->ConfChaussee.NumeroStation == vl_Num_RADT)
					{
						vl_Flag_Envoi = ((vl_Nb_Chaussees) == 0) ? ERDTC_DERN_MSG_TDC : vl_Nb_Chaussees;
						if ((vl_Cpt_Flag = sprintf(pl_MsgTDC, "%02d%lf %02d%03d%1d%1d%1d%03d%03d%s",
												   vl_Flag_Envoi, vl_Horodate, pg_Tab_RADT[vl_Num_RADT], vl_Num_RADT,
												   pl_ListeChaussee->ConfChaussee.Sens, vl_Index, vl_Index_Prec,
												   pg_DIV_Moy[0], pg_DIV_Moy[1], ERDTC_SEP_FLAG)) < 0)
						{
							XZST_03EcritureTrace(XZSTC_WARNING, "%s :  Erreur %d conversion chaine : <%s> pour envoi a TETDC ", version, vl_Cpt_Flag, pl_MsgTDC);
							XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s", version);
							return (XDC_NOK);
						}

						memcpy(pl_MsgTDC + vl_Cpt_Flag,
							   pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][pl_ListeChaussee->ConfChaussee.Sens],
							   sizeof(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][pl_ListeChaussee->ConfChaussee.Sens]));

						vl_Cpt_Flag += (int)sizeof(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT]][pl_ListeChaussee->ConfChaussee.Sens]);
						strcat(pl_MsgTDC + vl_Cpt_Flag + 2, XZEXC_FIN_MSG_SOCK);

						sem_wait(&vg_semaphore);
						if ((vl_retcode = (XDY_Entier)write(vg_SockTDC, pl_MsgTDC, (size_t)vl_Cpt_Flag + 2 + strlen(XZEXC_FIN_MSG_SOCK))) <= 0)
						{
							sem_post(&vg_semaphore);
							XZST_03EcritureTrace(XZSTC_WARNING, "%s :  Erreur envoi message socket TETDC ", version);
							XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s", version);
							return (XDC_NOK);
						}
						sem_post(&vg_semaphore);
						if (vl_Flag_Envoi != ERDTC_DERN_MSG_TDC)
							vl_Nb_Chaussees--;
						else
							break;
					}
				}
			}

			XZST_03EcritureTrace(XZSTC_INFO, "%s : fin traitement mesures 1 mn Q V TO", version);

			break;

			/*A ******** Cas des mesures 6mn %PL ******** */
		case XZEXC_TRAME_MES_6MN_PL:

			XZST_03EcritureTrace(XZSTC_INFO, "er_traitement_trame : Cas des mesures 6mn PL : St = %d Hr = %s trame = <%s>", vl_Num_RADT, pl_Horodatedecompo, pl_trame);
			/*B Si le status temps reel est different de "@"  => demande du status complet */
			if ((strstr(pl_trame, ERDTC_STATUS_TMP_REEL_STRG) == NULL) &&
				(vl_typ_rep != XZEXC_TRAME_NON_TRANSM) &&
				(vl_typ_rep != XZEXC_REP_NAK))
			{
				if (!strcmp(vl_station.ConfRADT.AdresseIP, XZECC_IP_VIDE))
					er_EnvoyerTrame("ST",
									XZEXC_TRAME_STAT_COMP,
									vl_adrrgs,
									XZEXC_NOCMD,
									XZEXC_PRIORITEFAIBLE,
									va_socket);
				else
					EnvoyerTrameIP("ST",
								   XZEXC_TRAME_STAT_COMP,
								   vl_station.ConfRADT.AdresseIP,
								   vl_station.ConfRADT.Port,
								   vl_station.ConfRADT.RGS,
								   XZEXC_NOCMD,
								   XZEXC_PRIORITEFAIBLE,
								   vl_station.Socket);
				strncpy(pl_trame, pl_trame, strlen(pl_trame) - 1);
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : Manque @", version);
			}
			else
				strtok(pl_trame, ERDTC_STATUS_TMP_REEL_STRG);

			/* Arrondi aux 6 min pleines */
			vl_Minutes = atoi(&pl_Horodatedecompo[ERDTC_DIXMINUTE_STRING]);
			vl_Result_Div = ldiv(vl_Minutes, 6);
			vl_Horodate = vl_Horodate - (vl_Result_Div.rem * 60);

			/*B Recherche de l'index du tableau de stockage des mesures 6mn PL de la station RADT */
			if ((vl_retcode = er_recherche_index(vl_Num_RADT, vl_minutes, ERDTC_6MINUTE, &vl_Index, &vl_Index_Prec)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_recherche_index retourne %d", version, vl_retcode);
				return (XDC_OK);
			}

			/*B JPL Initialisation des donnees PL de cette station a cette date (cet index) dans tous les cas */
			for (vl_Sens = 1; vl_Sens < ERDTC_NB_CHAUSSEE; (vl_Sens == 4) ? vl_Sens += 3 : vl_Sens++)
				for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
				}

			/*B Si Reponse de la station */
			if ((vl_typ_rep != XZEXC_TRAME_NON_TRANSM) && (vl_typ_rep != XZEXC_REP_NAK))
			{
				/*B S'il existe des sequences manquantes */
				if ((vl_Heure_Histo + 480) < vl_Horodate)
				{
					/*B Calcul du nombre de sequences manquantes et Demande d'acquisition de NB+2 sequences historiques */
					vl_Nb_Val_PL_Demand = (float)((vl_Horodate - vl_Heure_Histo) / 360);
					XZST_03EcritureTrace(XZSTC_INFO, "%s : St %d : Recherche 6 Mn PL His : Hr = %lf Hr_His = %lf",
										 version, vl_Num_RADT, vl_Horodate, vl_Heure_Histo);

					if (vl_Nb_Val_PL_Demand >= 720)
						vl_Nb_Val_PL_Demand = 720;
					if (vl_Nb_Val_PL_Demand == 0)
						vl_Nb_Val_PL_Demand = 1;
					sprintf(pl_Demand_His_PL, "MB %d LC", vl_Nb_Val_PL_Demand);

					XZST_03EcritureTrace(XZSTC_INFO, "%s : Recherche 6 Mn PL His avec %s, Horo = %lf ; Dern.Horo = %lf", version, pl_Demand_His_PL, vl_Horodate, vl_Heure_Histo);

					/*
					if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE))
							er_EnvoyerTrame(   pl_Demand_His_PL,
											   XZEXC_TRAME_MES_6MN_PL_HIS,
											   vl_adrrgs,
						   XZEXC_NOCMD,
											   XZEXC_PRIORITEFAIBLE,
											   va_socket);
				  else
				  EnvoyerTrameIP( pl_Demand_His_PL,
									 XZEXC_TRAME_MES_6MN_PL_HIS,
																pl_listeRADT->ConfRADT.AdresseIP,
																pl_listeRADT->ConfRADT.Port,
																 pl_listeRADT->ConfRADT.RGS,
																   XZEXC_NOCMD,
																XZEXC_PRIORITEFAIBLE,
									 pl_listeRADT->Socket );
					*/
					/*B Sortie */
					break;
				}

				/*B Appel de la fonction de traitement d'un trame => er_trait_commun */
				if ((vl_retcode = er_trait_commun(XZEXC_TRAME_MES_6MN_PL, vl_Num_LS, vl_Num_RADT, vl_TypeStation, vl_Horodate, vl_Index,
												  pl_trame, &vl_Horodate, &vl_Extrapolation)) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_trait_commun retourne : %d", version, vl_retcode);
				}

				/*A Si Plus d'une voie est manquantes => Extrapolation en fonction des stations amont et aval */
				/*DEM476 Mise en commentaire de la fonction d'extrapolation */
				/*if ( vl_Extrapolation.Nb_Extrapolation == 1 )
				{
					   XZST_03EcritureTrace ( XZSTC_INFO, "%s : appel fonction er_Extrapolation", version );

				   if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, vl_Extrapolation.Chaussee, vl_Extrapolation.Voie, ERDTC_6MINUTE, ERDTC_PL, vl_TypeStation ) ) != XDC_OK )
				   {
					  XZST_03EcritureTrace( XZSTC_WARNING, "%s : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
				   }
				}
				else
				{
				   if ( vl_Extrapolation.Nb_Extrapolation > 1 )
				   {
					  XZST_03EcritureTrace ( XZSTC_INFO, "%s : appel fonction er_Extrapolation", version );

					  if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, ERDTC_TOUS_SENS, ERDTC_TOUTES_VOIES, ERDTC_6MINUTE, ERDTC_PL, vl_TypeStation ) ) != XDC_OK )
					  {
					 XZST_03EcritureTrace( XZSTC_WARNING, "%s : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
					  }
				   }
				}*/
			} /* Fin Si Reponse */
			else
			/*B Sinon si Non Reponse */
			{
				XZST_03EcritureTrace(XZSTC_INFO, "%s : 6mn PL : aucune reponse sur la station : %d ", version, vl_Num_RADT);

				/*DEM476 Mise en commentaire de la fonction d'extrapolation */
				/*if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, ERDTC_TOUS_SENS, ERDTC_TOUTES_VOIES, ERDTC_6MINUTE, ERDTC_PL, vl_TypeStation ) ) != XDC_OK )
				{
				 XZST_03EcritureTrace( XZSTC_WARNING, "%s : 6min PL : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
				}*/

				/* Construction du nom du fichier Erreur_Trame si le fichier erreur_trame existe*/
				if (!access("erreur_trame", F_OK))
				{
					sprintf(pl_PathFicErr, "%s/Erreur_Trame_R%d", ERDTC_PATH_FIC_DYN, vl_Num_RADT);
					pl_Fd = fopen(pl_PathFicErr, "a+");
					XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
					fprintf(pl_Fd, "6 Mn PL : Hr %s St %d :: NON REPONSE\n", pl_Horo_Txt, vl_Num_RADT);
					fclose(pl_Fd);
				}
			}
			break;

		/*A ******** Cas des mesures 6mn Q, V, TO ******** */
		case XZEXC_TRAME_MES_6MN_Q_V_TO:

			XZST_03EcritureTrace(XZSTC_WARNING, "er_traitement_trame : Cas des mesures 6mn Q V TO : St = %d Hr = %s trame = <%s>", vl_Num_RADT, pl_Horodatedecompo, pl_trame);

			/*B Si le status temps reel est different de "@" => demande du status complet */
			if ((strstr(pl_trame, ERDTC_STATUS_TMP_REEL_STRG) == NULL) &&
				(vl_typ_rep != XZEXC_TRAME_NON_TRANSM) &&
				(vl_typ_rep != XZEXC_REP_NAK))
			{
				if (!strcmp(vl_station.ConfRADT.AdresseIP, XZECC_IP_VIDE))
					er_EnvoyerTrame("ST",
									XZEXC_TRAME_STAT_COMP,
									vl_adrrgs,
									XZEXC_NOCMD,
									XZEXC_PRIORITEFAIBLE,
									va_socket);
				else
					EnvoyerTrameIP("ST",
								   XZEXC_TRAME_STAT_COMP,
								   vl_station.ConfRADT.AdresseIP,
								   vl_station.ConfRADT.Port,
								   vl_station.ConfRADT.RGS,
								   XZEXC_NOCMD,
								   XZEXC_PRIORITEFAIBLE,
								   vl_station.Socket);
				strncpy(pl_trame, pl_trame, strlen(pl_trame) - 1);
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : Manque @", version);
			}
			else
				strtok(pl_trame, ERDTC_STATUS_TMP_REEL_STRG);

			/* Arrondi aux 6 min pleines */
			vl_Minutes = atoi(&pl_Horodatedecompo[ERDTC_DIXMINUTE_STRING]);
			vl_Result_Div = ldiv(vl_Minutes, 6);
			vl_Horodate = vl_Horodate - (vl_Result_Div.rem * 60);
			XZSM_11ConversionHorodate(vl_Heure_Histo, &vl_heure_string);

			/*B Recherche de l'index du tableau de stockage des mesures 6 min */
			if ((vl_retcode = er_recherche_index(vl_Num_RADT, vl_minutes, ERDTC_6MINUTE, &vl_Index, &vl_Index_Prec)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_recherche_index retourne %d", version, vl_retcode);
				return (XDC_OK);
			}

			/*B JPL Initialisation de toutes les donnees SAUF PL de cette station a cette date (cet index) dans tous les cas */
			for (vl_Sens = 1; vl_Sens < ERDTC_NB_CHAUSSEE; (vl_Sens == 4) ? vl_Sens += 3 : vl_Sens++)
				for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Horodate = vl_Horodate;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Sens][vl_Voie][vl_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
				}

			/*B Si Reponse de la station */
			if ((vl_typ_rep != XZEXC_TRAME_NON_TRANSM) && (vl_typ_rep != XZEXC_REP_NAK))
			{
				/*B Traitement des sequences manquantes */
				if ((vl_Heure_Histo + 480) < vl_Horodate)
				{
					/*B Si les mesures sont manquantes alors Calcul du nombre de sequences manquantes et
						Demande d'acquisition de NB+2 sequences historiques */
					vl_Nb_Val_QVTO_Demand = (float)((vl_Horodate - vl_Heure_Histo) / 360);

					if (vl_Nb_Val_QVTO_Demand >= 720)
						vl_Nb_Val_QVTO_Demand = 720;
					if (vl_Nb_Val_QVTO_Demand == 0)
						vl_Nb_Val_QVTO_Demand = 1;
					sprintf(pl_Demand_His_QVTO, "MB %d", vl_Nb_Val_QVTO_Demand);

					XZST_03EcritureTrace(XZSTC_INFO, "%s : Recherche 6 Mn QVTO St = %d avec %s, Horo = %lf ; Dern.Horo = %lf", version, vl_Num_RADT, pl_Demand_His_QVTO, vl_Horodate, vl_Heure_Histo);

					if (!strcmp(vl_station.ConfRADT.AdresseIP, XZECC_IP_VIDE))
						er_EnvoyerTrame(pl_Demand_His_QVTO,
										XZEXC_TRAME_MES_6MN_Q_V_TO_HIS,
										vl_adrrgs,
										XZEXC_NOCMD,
										XZEXC_PRIORITEFAIBLE,
										va_socket);
					else
						EnvoyerTrameIP(pl_Demand_His_QVTO,
									   XZEXC_TRAME_MES_6MN_Q_V_TO_HIS,
									   vl_station.ConfRADT.AdresseIP,
									   vl_station.ConfRADT.Port,
									   vl_station.ConfRADT.RGS,
									   XZEXC_NOCMD,
									   XZEXC_PRIORITEFAIBLE,
									   vl_station.Socket);

					/*JMG
							for ( pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL ; pl_ListeRADT = pl_ListeRADT->Suivant )
								 if ( pl_ListeRADT->ConfRADT.NumeroStation == vl_Num_RADT )
								 {
									pl_ListeRADT->Don_HisRADT.Demande_Histo = XDC_VRAI;
								break;
								 }*/
					break;
				}
				XZST_03EcritureTrace(XZSTC_INFO, "%s : er_trait_commun: 6 min QVTO NORMAL Station = %d , Horo = %s et Dern.Horo = %s", version, vl_Num_RADT, pl_Horodatedecompo, vl_heure_string);

				/*B Appel de la fonction de traitement d'un trame => er_trait_commun */
				if ((vl_retcode = er_trait_commun(XZEXC_TRAME_MES_6MN_Q_V_TO, vl_Num_LS, vl_Num_RADT, vl_TypeStation, vl_Horodate, vl_Index,
												  pl_trame, &vl_Horodate, &vl_Extrapolation)) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_trait_commun retourne : %d", version, vl_retcode);
				}

				/*B Si Plus d'une voie est manquantes => Extrapolation en fonction des stations amont et aval */
				/*DEM476 Mise en commentaire de la fonction d'extrapolation */
				/*if ( vl_Extrapolation.Nb_Extrapolation == 1 )
				{
				   XZST_03EcritureTrace ( XZSTC_INFO, "%s : appel fonction er_Extrapolation", version );

				   if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, vl_Extrapolation.Chaussee, vl_Extrapolation.Voie, ERDTC_6MINUTE, ERDTC_NON_PL, vl_TypeStation ) ) != XDC_OK )
				   {
					  XZST_03EcritureTrace( XZSTC_WARNING, "%s : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
				   }
				}
				else
				{
				   if ( vl_Extrapolation.Nb_Extrapolation > 1 )
				   {
					  XZST_03EcritureTrace ( XZSTC_INFO, "%s : appel fonction er_Extrapolation", version );

					  if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, ERDTC_TOUS_SENS, ERDTC_TOUTES_VOIES, ERDTC_6MINUTE, ERDTC_NON_PL, vl_TypeStation ) ) != XDC_OK )
					  {
					 XZST_03EcritureTrace( XZSTC_WARNING, "%s : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
					  }
				   }
				}*/
				if (vl_Extrapolation.Nb_Extrapolation <= 1)
				{
					/*B Calcul des donnees par chaussee => Appel de la fonction er_Calcul_Chausssee */
					XZST_03EcritureTrace(XZSTC_DEBUG2, "%s : 6min QVTO : Appel er_Calcul_Chaussee", version);
					XZST_03EcritureTrace(XZSTC_INFO, "Appel de la fonction er_Calcul_Chausssee: %s : 6min QVTO : RADT = %d\tIndex = %d", version, vl_Num_RADT, vl_Index);
					if ((vl_retcode = er_Calcul_Chaussee(vl_Num_RADT, vl_TypeStation, vl_Index, vl_Horodate)) != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min QVTO : er_Calcul_Chaussee retourne %d", version, vl_retcode);

						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Horodate = vl_Horodate;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT]][vl_Chaussee][0][vl_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
					}
				}
			} /* Fin Si Reponse */
			else
			/* Sinon Si Non Reponse */
			{
				XZST_03EcritureTrace(XZSTC_INFO, "%s 6mn Q V TO : aucune reponse ", version);

				/*DEM476 Mise en commentaire de la fonction d'extrapolation */
				/*if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, ERDTC_TOUS_SENS, ERDTC_TOUTES_VOIES, ERDTC_6MINUTE, ERDTC_NON_PL, vl_TypeStation ) ) != XDC_OK )
			{
			   XZST_03EcritureTrace( XZSTC_WARNING, "%s : 6min QVTO : Extrapolation de mesures impossible pour la station : %d", version, vl_Num_RADT);
			}*/

				/* Construction du nom du fichier Erreur_Trame si le fichier erreur_trame existe */
				if (!access("erreur_trame", F_OK))
				{
					sprintf(pl_PathFicErr, "%s/Erreur_Trame_R%d", ERDTC_PATH_FIC_DYN, vl_Num_RADT);
					pl_Fd = fopen(pl_PathFicErr, "a+");
					XZSM_11ConversionHorodate(vl_Horodate, &pl_Horo_Txt);
					fprintf(pl_Fd, "6 Mn Q V TO : Hr %s St %d :: NON REPONSE\n", pl_Horo_Txt, vl_Num_RADT);
					fclose(pl_Fd);
				}
			}

			/*B Envoi des donnees IHM tisyn => Appel de la fonction er_Env_Mes_6mn */
			if ((vl_retcode = er_Env_Mes_6mn(vl_Num_RADT, vl_Index)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min QVTO : er_Env_Mes_6mn retourne %d", version, vl_retcode);
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_INFO, "%s : 6min QVTO : er_Env_Mes_6mn retourne %d", version, vl_retcode);
			}

			/*B Si LT = PC simplifie : Calcul et envoi des niveaux de services
				=> Appel de la fonction er_Niveau_Service du module MCAL */
			if (vg_Mac_Ext_Ouest)
			{
				XZST_03EcritureTrace(XZSTC_DEBUG2, "%s : appel fonction er_Niveau_Service", version);

				if ((vl_retcode = er_Niveau_Service(vl_Num_RADT, vl_TypeStation, vl_Index, vl_Horodate, ERDTC_6MINUTE)) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_Niveau_Service retourne : %d", version, vl_retcode);
				}
			}

			/*B Envoi des donnees par chaussee => Appel de la fonction er_Stockage_Mesures */
			if ((vl_retcode = er_Stockage_Mesures(vl_Num_RADT, vl_Index, vl_Horodate,
												  ((vl_typ_rep != XZEXC_TRAME_NON_TRANSM) && (vl_typ_rep != XZEXC_REP_NAK)) ? XDC_VRAI : XDC_FAUX)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min QVTO : er_Stockage_Mesures retourne %d", version, vl_retcode);
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_INFO, "DEM476 %s : 6min QVTO OK : er_Stockage_Mesures retourne %d", version, vl_retcode);
			}

			/*B Envoi des donnees par chaussee => Appel de la fonction er_Fichier_Mesures */
			if ((vl_retcode = er_Fichier_Mesures(vl_Num_RADT, vl_Index, vl_Horodate)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min QVTO : er_Fichier_Mesures retourne %d", version, vl_retcode);
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_INFO, "DEM476 %s : 6min QVTO OK: er_Fichier_Mesures retourne %d", version, vl_retcode);
			}

			/*B Envoi des donnees par chaussee => Appel de la fonction er_Envoi_Mesure */
			if (vg_Etat_Liaison_CI == ERDTC_LIAISON_CI_OK)
			{
				if ((vl_retcode = er_Envoi_Mesure(vl_Num_RADT, XDC_FAUX)) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min QVTO : er_Envoi_Mesure retourne %d", version, vl_retcode);
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_INFO, "DEM476 %s : 6min QVTO OK: er_Envoi_Mesure retourne %d", version, vl_retcode);
				}
			}
			else
				XZST_03EcritureTrace(XZSTC_WARNING, "Pas de liaison avec le CI : RADT %s",vl_Num_RADT);

			XZST_03EcritureTrace(XZSTC_INFO, "%s : Fin Traitement 6 Mn QVTO ", version);
			break;

		/*A ******** Cas des mesures individuelles ******** */
		case XZEXC_TRAME_CLASSE_DIV:

			XZST_03EcritureTrace(XZSTC_INFO, "IN : %s : Cas des mesures Individuelles : trame = %s", version, pl_trame);

			/*B Si le status temps reel est different de "@" => demande du status complet */
			if ((strstr(pl_trame, ERDTC_STATUS_TMP_REEL_STRG) == NULL) &&
				(vl_typ_rep != XZEXC_TRAME_NON_TRANSM) &&
				(vl_typ_rep != XZEXC_REP_NAK))
			{
				if (!strcmp(vl_station.ConfRADT.AdresseIP, XZECC_IP_VIDE))
					er_EnvoyerTrame("ST",
									XZEXC_TRAME_STAT_COMP,
									vl_adrrgs,
									XZEXC_NOCMD,
									XZEXC_PRIORITEFAIBLE,
									va_socket);
				else
					EnvoyerTrameIP("ST",
								   XZEXC_TRAME_STAT_COMP,
								   vl_station.ConfRADT.AdresseIP,
								   vl_station.ConfRADT.Port,
								   vl_station.ConfRADT.RGS,
								   XZEXC_NOCMD,
								   XZEXC_PRIORITEFAIBLE,
								   vl_station.Socket);
				strncpy(pl_trame, pl_trame, strlen(pl_trame) - 1);
				XZST_03EcritureTrace(XZSTC_WARNING, "%s : Manque @", version);
			}
			else
				strtok(pl_trame, ERDTC_STATUS_TMP_REEL_STRG);

			vl_pas_pointeur = 0;
			vl_longueur = (XDY_Entier)strlen(pl_trame);

			/*B Si Non Reponse de la station */
			if ((vl_typ_rep == XZEXC_TRAME_NON_TRANSM) || (vl_typ_rep == XZEXC_REP_NAK))
			{
				break;
			}
			else
			/*B Sinon s'il existe une reponse => traitement de la trame recue */
			{
				/*B Recherche de la valeur du champ DonneesIndividuelles pour la station RADT */
				vl_Mesures_Individuelles = vl_station.ConfRADT.DonneesIndividuelles;

				/*A Conversion de l'horodate recue */
				sscanf(vl_horod2, "%02d:%02d:%02d", &vl_heures, &vl_minutes, &vl_secondes);
				sprintf(pl_Horodatedecompo, "%s %02d:%02d:%s", vl_horod1, vl_heures, vl_minutes, (vl_secondes < 30) ? "00" : "30");
				XZSM_13ConversionHorodateSec(pl_Horodatedecompo, &vl_horodate);

				/*B Decodage de la trame par voie pour trouver : le Numero de canal, le temps de passage en secondes,
					le temps de passage en centiemes de secondes, la vitesse en km/h, la longueur du vehicule en dm,
					la silhouette en decimal et le poids total en dixieme de tonne
					=> Extraction des differentes sequences recues pour les 30 secondes ecoulees */

				strcpy(pl_trame_mesure, "   ");

				vl_Num_Boucle_str = pl_trame[0];
				pl_trame_mesure[0] = pl_trame[0];

				/*fprintf ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_Num_RADT]], "%s\n", pl_trame );*/

				/*B Lecture de la trame recue */
				while (vl_pas_pointeur < (vl_longueur - 1))
				{
					vl_Nb_Param = 0;
					vl_Mes_Manquante = XDC_FAUX;
					vl_Continue = XDC_FAUX;
					vl_DIV = 0;
					vl_Capteur_Poids = 0;
					vl_Poids = 0;
					vl_Nb_Essieu = 0;
					pl_trame_mesure[0] = pl_trame[vl_pas_pointeur];
					sscanf(pl_trame_mesure, "%1d", &vl_Num_Voie);

					/*B Recherche si le Point de Mesure possede un capteur de poids */
					for (pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant)
					{
						/*B Stockage de l'etat des voies de la station RADT dans la variable locale vl_Etat_Voie */
						if ((pl_ListePtMes->ConfPtMes.NumeroStation == vl_Num_RADT) && ((pl_ListePtMes->ConfPtMes.Boucle == vl_Num_Voie) || (pl_ListePtMes->ConfPtMes.BoucleInv == vl_Num_Voie)))
						{
							vl_Num_Voie = pl_ListePtMes->ConfPtMes.Voie;
							vl_SensVoie = pl_ListePtMes->ConfPtMes.Sens;
							vl_Capteur_Poids = pl_ListePtMes->ConfPtMes.Poids;
							/*B Si la voie possede un capteur de poids lecture de 16 caracteres de la trame : mssccvvvlllkkpppn(+ n fois eee) */
							if (vl_Capteur_Poids == XDC_VRAI)
							{
								vl_ajt_pointeur = 17;
								vl_Continue = XDC_FAUX;
							}
							else
							/*B Si la voie ne possede pas de capteur de poids lecture de 11 caracteres de la trame : mssccvvvlll */
							{
								if (pl_ListePtMes->ConfPtMes.Boucle != pl_ListePtMes->ConfPtMes.BoucleInv)
								{
									vl_ajt_pointeur = 11;
									vl_Continue = XDC_FAUX;
								}
								else
								{
									vl_ajt_pointeur = 9;
									vl_Continue = XDC_VRAI;
								}
							}
							break;
						}
						else
						{
							vl_Continue = XDC_VRAI;
							vl_ajt_pointeur = 11;
						}
					}

					/*B Stockage d'une mesure dans une variable intermedaire : pl_trame_mesure */
					strncpy(pl_trame_mesure, pl_trame + vl_pas_pointeur, vl_ajt_pointeur);
					pl_trame_mesure[vl_ajt_pointeur] = '\0';
					for (vl_i = 0; vl_i < vl_ajt_pointeur - 1; vl_i++)
					{
						if (pl_trame_mesure[vl_i] == ' ')
							pl_trame_mesure[vl_i] = '0';
						if (pl_trame_mesure[vl_i] < '0' || pl_trame_mesure[vl_i] > '9')
							vl_Continue = XDC_VRAI;
					}

					/*B Si la variable vl_Continue est positionnee => pas de stockage des mesures recues
							<=> la voie n'existe pas pour la station */
					if (vl_Continue == XDC_VRAI)
					{
						vl_pas_pointeur += vl_ajt_pointeur;
						continue;
					}

					sscanf(pl_trame_mesure, "%1d%2d%2d%3d%2d%1d%2d%3d%1d", &vl_Voie, &vl_Sec, &vl_mSec, &vl_Vit, &vl_Long_metre, &vl_Long_decim, &vl_Silhouette, &vl_Poids, &vl_Nb_Essieu);

					/*	fprintf ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_Num_RADT]], "%s\n", pl_trame_mesure );*/
					XZST_03EcritureTrace(XZSTC_DEBUG2, "Dem Indiv : Voie = %d\tSec. = %d\t mSec. = %ddt\tVit. = %d\tLong. =%d\tSilh. =%d\tPoids =%d\tEssieu =%d\tindex =%d",
										 vl_Voie, vl_Sec, vl_mSec, vl_Vit, vl_Long_metre, vl_Silhouette, vl_Poids, vl_Nb_Essieu, vl_pas_pointeur);

					vl_Poids = (vl_Poids * 10);

					/*B Calcul des DIV pour la classe du vehicule et mise a jour du tableau de stockage des mesures */
					vl_SecondePassage = (float)(vl_Sec + (float)(vl_mSec / 100.0));
					vl_SecondePassage = (vl_SecondePassage > 30) ? (vl_SecondePassage - 30) : vl_SecondePassage;
					if (pl_ListePtMes != NULL)
					{
						if (pl_ListePtMes->Sec_dernier_veh > 0)
						{
							vl_TempInterVeh = vl_horodate - pl_ListePtMes->Sec_dernier_veh + vl_SecondePassage;
							pl_ListePtMes->Sec_dernier_veh = vl_horodate + vl_SecondePassage;
							vl_DIV = (double)((double)(1000.0 * vl_Vit * vl_TempInterVeh) / 3600.0);
						}
						else
						{
							pl_ListePtMes->Sec_dernier_veh = vl_horodate + vl_SecondePassage;
							vl_DIV = 0;
							vl_TempInterVeh = 0;
						}
					}

					pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_SensVoie].Cumul += vl_Poids;

					XZST_03EcritureTrace(XZSTC_DEBUG3, "Dem Indiv : Poids = %d", vl_Poids);

					/*B Si l'indicateur mesures individuelles est positionne stockage dans un fichier */
					if (vg_Demande_Individuelle[pg_Tab_RADT[vl_Num_RADT]] == XDC_VRAI)
					{
						sprintf(vl_Msg, "%d\t%d\t%d\t%s\t%s\t%d\t%d\t%.0lf\t%d,%01d\t%.2lf\t%d,%02d\0",
								vl_Num_RADT, vl_SensVoie, vl_Num_Voie, vl_horod1, vl_horod2,
								vl_Vit, vl_Poids, vl_DIV, vl_Long_metre, vl_Long_decim, vl_TempInterVeh, vl_Sec, vl_mSec);

						for (vl_i = 0; vl_Msg[vl_i] != '\0'; vl_i++)
							if (vl_Msg[vl_i] == '.')
								vl_Msg[vl_i] = ',';
						fprintf(vg_Fd_Dem_Ind[pg_Tab_RADT[vl_Num_RADT]], "%s\n", vl_Msg);

						/*B Si le fichier est plein (1 heure) fermer le fichier et repositionner l'indicateur de demandes individuelles a faux */
						if (vg_Horo_Dem_Ind[pg_Tab_RADT[vl_Num_RADT]] + 43200 <= vl_Horodate)
						{
							vg_Demande_Individuelle[pg_Tab_RADT[vl_Num_RADT]] = XDC_FAUX;
							fclose(vg_Fd_Dem_Ind[pg_Tab_RADT[vl_Num_RADT]]);
							vg_Fd_Dem_Ind[pg_Tab_RADT[vl_Num_RADT]] = 0;
						}
					}

					vl_pas_pointeur = vl_pas_pointeur + vl_ajt_pointeur + (((vl_Capteur_Poids == XDC_VRAI) && (vl_Nb_Essieu > 0)) ? (3 * vl_Nb_Essieu) : 0);
				} /*fin while */
			}
			/*B Si l'indicateur mesures individuelles est positionne stockage dans un fichier */
			if (vg_Demande_Individuelle[pg_Tab_RADT[vl_Num_RADT]] == XDC_VRAI)
				fflush(vg_Fd_Dem_Ind[pg_Tab_RADT[vl_Num_RADT]]);
			break;

		/*A ******** Cas des mesures 6mn Q, V, TO Historisees ******** */
		case XZEXC_TRAME_MES_6MN_Q_V_TO_HIS:

			XZST_03EcritureTrace(XZSTC_INFO, "6mn QVTO His : St = %d   LONGUEUR = %d LS = %d", vl_Num_RADT, strlen(pl_trame), vl_Num_LS);

			/* Sauvegarde donnees radt */
			/*JMG pl_ListeRADT->Don_HisRADT.Demande_Histo = XDC_FAUX;
		break;*/
			/*B Sinon Si Reponse de la station => Traitement de la trame */
			/*B Recherche de l'index des dernieres mesures stockees pour la station RADT */
			if ((vl_retcode = er_dernier_index(vl_Num_RADT, ERDTC_6MINUTE, &vl_Index_Prec, &vl_Horodate_Prec)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "6mn QVTO His : er_dernier_index retoune %d ", vl_retcode);
				break;
			}

			if ((vl_typ_rep != XZEXC_TRAME_NON_TRANSM) && (vl_typ_rep != XZEXC_REP_NAK) ||
				(strlen(pl_trame) > 1000))
			{
				/*B Appel de la fonction er_trait_commun avec l'horodate historique */
				if ((vl_retcode = er_trait_commun(XZEXC_TRAME_MES_6MN_Q_V_TO_HIS, vl_Num_LS, vl_Num_RADT, vl_TypeStation,
												  vl_Heure_Histo, vl_Index, pl_trame, &vl_Horodate_Histo,
												  &vl_Extrapolation)) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_trait_commun retourne : %d", version, vl_retcode);
				}

				if ((vl_station.Don_HisRADT.Horodate_Histo + 360) < vl_Horodate)
				{
					/*B Recherche de l'index du tableau de stockage des mesures 6 min */
					if ((vl_retcode = er_recherche_index(vl_Num_RADT, vl_minutes, ERDTC_6MINUTE, &vl_Index, &vl_Index_Prec)) != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "%s : er_recherche_index retourne %d", version, vl_retcode);
						return (XDC_OK);
					}
					/* Arrondi aux 6 min pleines */
					vl_Minutes = atoi(&pl_Horodatedecompo[ERDTC_DIXMINUTE_STRING]);
					vl_Result_Div = ldiv(vl_Minutes, 6);
					vl_Horodate = vl_Horodate - (vl_Result_Div.rem * 60);
					/*DEM476 Mise en commentaire de la fonction d'extrapolation */
					/*if ( ( vl_retcode = er_Extrapolation ( vl_Index, vl_Index_Prec, vl_Horodate, vl_Num_RADT, ERDTC_TOUS_SENS, ERDTC_TOUTES_VOIES, ERDTC_6MINUTE, ERDTC_NON_PL, vl_TypeStation ) ) != XDC_OK )
						{
					   XZST_03EcritureTrace( XZSTC_WARNING, "6mn QVTO His : Extrapolation de mesures impossible pour la station : %d", vl_Num_RADT);
						}*/
				}

				/*B Envoi des donnees IHM tisyn => Appel de la fonction er_Env_Mes_6mn */
				er_Env_Mes_6mn(vl_Num_RADT, vl_Index);

				/*B Envoi des donnees => Appel de la fonction er_Envoi_Mesure */
				if (vg_Etat_Liaison_CI == ERDTC_LIAISON_CI_OK)
				{
					if ((vl_retcode = er_Envoi_Mesure(vl_Num_RADT, XDC_FAUX)) != XDC_OK)
						XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min QVTO His : er_Envoi_Mesure retourne %d", version, vl_retcode);
				}
				else
					XZST_03EcritureTrace(XZSTC_WARNING, "Pas de liaison avec le CI : RADT %s",vl_Num_RADT);
			}

			vg_Rep_His_PL[vl_Num_LS] = XDC_FAUX;
			strcpy(pg_Trame[vl_Num_LS], "");
			strcpy(pg_Trame_PL[vl_Num_LS], "");
			break;

		/*A ******** Cas des mesures 6mn PL Historisees ******** */
		case XZEXC_TRAME_MES_6MN_PL_HIS:

			XZST_03EcritureTrace(XZSTC_INFO, "6mn PL His : St = %d   LONGUEUR = %d LS = %d ", vl_Num_RADT, strlen(pl_trame), vl_Num_LS);

			/*B Si Non Reponse de la station */
			if ((vl_typ_rep == XZEXC_TRAME_NON_TRANSM) || (vl_typ_rep == XZEXC_REP_NAK))
			{
				vg_Rep_His_PL[vl_Num_LS] = XDC_FAUX;
				strcpy(pg_Trame_PL[vl_Num_LS], "");
			}
			/*B Sinon Si Reponse de la station => Traitement de la trame */
			else
			{
				vg_Rep_His_PL[vl_Num_LS] = XDC_VRAI;
				strcpy(pg_Trame_PL[vl_Num_LS], pl_trame);
			}
			break;
		/*A ******** Cas des d�bits par classifi�s ******** */
		case XZEXC_TRAME_PESAGE_SILHOUETTE:
		case XZEXC_TRAME_PESAGE_ESSIEU:
		case XZEXC_TRAME_PESAGE_POIDS:

			strncpy(pl_trame, pl_trame, strlen(pl_trame) - 1);

			/* Arrondi de l'horodate aux heures pleines */
			sprintf(pl_Horodatedecompo, "%s %02d:00:01", vl_horod1, vl_heures);
			XZSM_13ConversionHorodateSec(pl_Horodatedecompo, &vl_horodate);

			if ((vl_typ_rep == XZEXC_TRAME_NON_TRANSM) || (vl_typ_rep == XZEXC_REP_NAK))
				vl_Mes_Manquante = XDC_VRAI;

			/*B mise a jour du pointeur en fonction du nombre de classe */
			switch (vl_typetrame)
			{
			case XZEXC_TRAME_PESAGE_SILHOUETTE:
				vl_Pesage.NbClasse = XDC_NB_CLASSE_SILHOUETTE;
				vl_Pesage.Type = XDC_MESURE_SILHOUETTE;
				pl_HistoPesage = &(vl_station.Don_HisSilhouette);
				XZST_03EcritureTrace(XZSTC_INFO, "IN : Traitement des d�bits par classe de silhouette");
				break;
			case XZEXC_TRAME_PESAGE_ESSIEU:
				vl_Pesage.NbClasse = XDC_NB_CLASSE_ESSIEU;
				vl_Pesage.Type = XDC_MESURE_ESSIEU;
				pl_HistoPesage = &(vl_station.Don_HisEssieu);
				XZST_03EcritureTrace(XZSTC_INFO, "IN : Traitement des d�bits par classe de poids d'essieu");
				break;
			case XZEXC_TRAME_PESAGE_POIDS:
				vl_Pesage.NbClasse = XDC_NB_CLASSE_POIDS;
				vl_Pesage.Type = XDC_MESURE_POIDS;
				pl_HistoPesage = &(vl_station.Don_HisPoids);
				XZST_03EcritureTrace(XZSTC_INFO, "IN : Traitement des d�bits par classe de poids total");
				break;
			}
			vl_pas_pointeur = 0;
			vl_longueur = (XDY_Entier)strlen(pl_trame);
			vl_ajt_pointeur = vl_Pesage.NbClasse * 5;
			vl_Pesage.NumeroStation = vl_Num_RADT;

			for (vl_Sens = 0; vl_Sens < ERDTC_NB_CHAUSSEE; vl_Sens++)
				for (vl_Num_Voie = 0; vl_Num_Voie < ERDTC_NB_VOIE_CHAUS; vl_Num_Voie++)
				{
					vl_Pesage.Mesure[vl_Sens][vl_Num_Voie].Voie = XDC_VOIE_INCONNUE;
					for (vl_Index = 0; vl_Index < vl_Pesage.NbClasse; vl_Index++)
						vl_Pesage.Mesure[vl_Sens][vl_Num_Voie].Classe[vl_Index] = XDC_RADT_Q_INCONNU;
				}

			/*B Recherche si il existe des Point de Mesure avc un capteur de poids */
			for (pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant)
				/*B Stockage de l'etat des voies de la station RADT dans la variable locale  */
				if ((pl_ListePtMes->ConfPtMes.NumeroStation == vl_Num_RADT) && (pl_ListePtMes->ConfPtMes.Poids))
					vl_Pesage.Mesure[pl_ListePtMes->ConfPtMes.Sens][pl_ListePtMes->ConfPtMes.Voie].Voie = pl_ListePtMes->ConfPtMes.Voie;

			if (!vl_Mes_Manquante)
			{
				for (vl_Num_Boucle = 1; vl_Num_Boucle <= pl_HistoPesage->Demande_Histo; vl_Num_Boucle++)
				{
					/*B Lecture de la trame recue */
					for (vl_Sens = 0; vl_Sens < ERDTC_NB_CHAUSSEE; vl_Sens++)
						for (vl_Num_Voie = 0; vl_Num_Voie < ERDTC_NB_VOIE_CHAUS; vl_Num_Voie++)
							if (vl_Pesage.Mesure[vl_Sens][vl_Num_Voie].Voie != XDC_VOIE_INCONNUE)
							{
								/*B Stockage d'une mesure dans une variable intermedaire : pl_trame_mesure */
								strncpy(pl_trame_mesure, pl_trame + vl_pas_pointeur, vl_ajt_pointeur);
								pl_trame_mesure[vl_ajt_pointeur] = '\0';
								vl_pas_pointeur += vl_ajt_pointeur;
								for (vl_Index = 0; vl_Index < vl_Pesage.NbClasse; vl_Index++)
									vl_Nb_Param = sscanf(pl_trame_mesure + (vl_Index * 5), "%5d", &vl_Pesage.Mesure[vl_Sens][vl_Num_Voie].Classe[vl_Index]);
								vl_Pesage.Mesure[vl_Sens][vl_Num_Voie].Classe[vl_Index] = (vl_Nb_Param == 1) ? vl_Pesage.Mesure[vl_Sens][vl_Num_Voie].Classe[vl_Index] : XDC_RADT_Q_INCONNU;
							}

					vl_Pesage.Horodate = vl_horodate - (3600 * (pl_HistoPesage->Demande_Histo - vl_Num_Boucle));
					pl_HistoPesage->Horodate_Histo = vl_Pesage.Horodate + 3600;
					er_Stockage_Pesage(vl_Pesage, XDC_VRAI);
					if ((vl_Num_Boucle == 1) && (vg_Etat_Liaison_CI == ERDTC_LIAISON_CI_OK))
						er_Envoi_Mesure_Pesage(vl_Num_RADT, XDC_FAUX, vl_Pesage.Type);
					if (vl_pas_pointeur > vl_longueur)
						break;
				}
				pl_HistoPesage->Dern_Index = 0;
			}
			else
			{
				if (pl_HistoPesage->Demande_Histo != pl_HistoPesage->Dern_Index)
				{
					vl_Pesage.Horodate = vl_horodate;
					er_Stockage_Pesage(vl_Pesage, XDC_FAUX);
					if (vg_Etat_Liaison_CI == ERDTC_LIAISON_CI_OK)
						er_Envoi_Mesure_Pesage(vl_Num_RADT, XDC_FAUX, vl_Pesage.Type);
					pl_HistoPesage->Dern_Index = pl_HistoPesage->Demande_Histo;
				}
			}
			break;
		}
	}
	return (XDC_OK);
}
