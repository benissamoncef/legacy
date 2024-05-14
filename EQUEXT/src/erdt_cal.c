/*E*/
/*Fichier : $Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $      Release : $Revision: 1.67 $        Date : $Date: 2021/01/29 14:49:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_cal.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  < Module de calcul des donnees de comptage de la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	07 Dec 1994	: Creation
* Volcic.F	version 1.2	28 Jan 1995	:
* Volcic.F	version 1.3	28 Jan 1995	:
* Volcic.F	version 1.4	01 Fev 1995	: Ajout Envoi Message d'alerte pour un chgt de niveau de service
* Volcic.F	version 1.5	20 Mar 1995	: Modifs Diverses
* Volcic.F	version 1.6	20 Mar 1995	: Modifs Diverses
* Volcic.F	version 1.7	20 Mar 1995	: Modifs Calcul 6gl Chaussee
* Volcic.F	version 1.8	19 Avr 1995	:
* Volcic.F	version 1.9	21 Avr 1995	: Reprise historique,
* Volcic.F	version 1.10	04 Jui 1995	: Ajout de la fonction er_Envoi_Mesures_HIS
* Volcic.F	version 1.11	27 Sep 1995	: Suppr. des envois fichier en mode isole
* Volcic.F	version 1.17	11 Dec 1995	: Suppr. commentaires & Ajout pas envoi Niveau de service si station != chaussee
* Volcic.F	version 1.18	23 Jan 1996	: Ajout init valeurs pour extrapolation station chaussee+echangeur
* Volcic.F	version 1.19	26 Fev 1996	: Correction changement de contexte jour/nuit pour  er_Niveau_service (DEM/1020)
* Volcic.F	version 1.20	07 Mai 1996	: Suppression ajout entr�e et sortie sur station �changeur (DEM/1019)
*						  Mise a jour indicateur extrapol�e pour donn�e 6mn glissante (DEM/1015)
* Volcic.F	version 1.21	14 Mai 1996	: Correction %PL pb sur correction (DEM/1015)
* Volcic.F      version 1.22    14 Mai 1996     : Correction er_mesures_valides pb sur correction (DEM/1017)
* Volcic.F      version 1.23    28 Mai 1996     : Correction pour raf tps reel voie neutralisee (DEM/1050)
*						: RAZ des donn�es sous travaux dans er_Fichier_Mesures et er_Stockage_Mesures (DEM/1032)
* Mismer.D      version 1.24    15 Jul 1996     : Correction pour pb trop de station (DEM/1150)
* Volcic.F	version 1.25	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.26	16 Sep 1996	: Modif Extension Ouest Suite (RADT_W)
* Mismer.D	version 1.27	19 Sep 1996	: Modif stockage donnee (RADT_W)
* Mismer.D	version 1.28	02 Sep 1996	: Modif pour mesures pesage (DEM/1227)
* Mismer.D	version 1.29	15 Oct 1996	: Modif pour envoi mesure extrapol� (DEM/1227)
* Mismer.D	version 1.30	21 Oct 1996	: Modif station virtuelle EQT12 (DEM/1252)
* Mismer.D	version 1.31	10 Dec 1996	: Correction raf voie bretelle des stations virtuelles EQT12 (DEM/1252)
* Mismer.D	version 1.32	10 Dec 1996	: Correction PL  des stations virtuelles EQT12 (DEM/1252)
* Mismer.D	version 1.33	12 Dec 1996	: Calcul des niveaux de services pourstations virtuelles (DEM/1338)
* Mismer.D      version 1.34    16 Dec 1996     : Modif rafraichissement syno sur reprise com (DEM/1291/1)
* Mismer.D	version 1.35	22 Jan 1998	: Modif raf syno (DEM/1543)
* Mismer.D	version 1.36	30 Jan 1998	: Modif raf syno suite (DEM/1543)
* Mismer.D	version 1.37	10 Mar 1998	: Modif raf syno suite suite (DEM/1543)
* Niepceron	version 1.38	09 Fev 1999	: Mise en base station virtuelle
* Cluchague	version 1.40	11 Mar 2002	: Inhibition alerte depassement niveau trafic (pour nice) lgn 2383
						   et ajout trace pour donnes 1mn et ajout trace pour alerte pour analyse
* Hilmarcher    version 1.41    27 oct 2004     : Modif pour correction pourcentage poids lourds temps reel (DEM/298)
* Hilmarcher    version 1.42    10 mai 2005     : Rajout prise en compte alerte DEM465
* Hilmarcher    version 1.43    22 juin 2005    : inhibition de la fonction d'extrapolation des donnes 6mn DEM476
* Hilmarcher    version 1.44    07 juill  05    : correction bug pas d'enregistrement en base qd sens != 1 ou  2
* Niepceron	version 1.45	04 oct 2006	: Correction bug : qd Q de voie 2 est nulle , la donn�e de la chaussee �tait mise en base DEM590
* Niepceron	version 1.46	26 jan 2007	: Ajout du filtre sur la plage horaire avant enregistrement; La suppression des enregt de donn�es chauss�es < 50 ne doit se faire que durant la plage 7h30 - 22h DEM612
* Niepceron	version 1.47	29 Aou 2007	: Seules Les donn�es Q(chauss�e) RADT des sens 1 et 2 doivent �tre filtr�es sur Q(chauss�e)<50 DEM696
* JMG		13/11/07 : ajout site de gestion DEM715
* JMG		29/01/08 : Modif pour envoi des donn�es suivant site de gestion DEM748 1.49
* JPL		10/07/08 : Si Q(voie normale)<=0 entre 07:30 et 22:00 sens 1 ou 2, valeurs chaussee IHM = -1 (DEM 681) 1.50
* JPL		24/11/09 : Changement de niveaux de traces, notamment "Warning" en "Info" (DEM 908) 1.51
* JMG		26/06/10 : portage linux DEM/934
* JPL		17/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.53
* JPL		17/03/11 : Migration HP ia64 (DEM 975) : Correction trace er_Calcul_Chaussee; retour C.R. fonctions  1.54
* JPL		17/03/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=,+)  1.55
* JPL		19/12/11 : Ne plus considerer une chausee (un sens) de numero 5  1.56
* JPL		19/12/11 : Suppression avertissements de compilation mineurs  1.57
* JPL		19/12/11 : Valuation de l'horodate des donnees extrapolees (DEM 997)  1.58
* JMG		04/07/12 : regles de qualif DEM 1016 1.59
* JPL		07/02/13 : Calcul chaussee : positionner indicateur de validite %PL  1.60
* JMG		03/04/13 : correction regle XDC_REG_SEUIL_DEBIT_NUL
* JMG		15/06/13 : correction station virtuelle
* SFO           06/10/14 : Adaptation pour compilation sous linux
* PNI		1/02/16  : Correction pour filtre "une voie nulle de jour alors chaussee invalide" DEM1149 v1.63
* PNI		12/12/16 : Prise en compte des vitesses/PL pour les nouvelles stations en �changeur V1.64 DEM1213
* PNI		06/02/19 : Test si station HS avant calcul/envoi des niveaux de trafic v1.65
* AMI    28/01/21 : SAE_191: chantier date
------------------------------------ ------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include <stdio.h>
#include <stdlib.h>

#include "ex_mala.h"

#include "erdt_cal.h"
#include "erdt_tra.h"

/* definitions de constantes */

/* definitions de types locaux */
extern XDY_Booleen vg_mode_simu;

/* definition de macro locales */

/* declaration de variables locales */
static XDY_Moyenne pm_coef_Extra_Q[ERDTC_NB_STATION][3][ERDTC_NB_VOIE_CHAUS];
static XDY_Moyenne pm_coef_Extra_V[ERDTC_NB_STATION][3][ERDTC_NB_VOIE_CHAUS];
static XDY_Moyenne pm_coef_Extra_TO[ERDTC_NB_STATION][3][ERDTC_NB_VOIE_CHAUS];
static XDY_Moyenne pm_coef_Extra_PL[ERDTC_NB_STATION][3][ERDTC_NB_VOIE_CHAUS];

static XDY_Coeff vm_Coef_a = 0.0;
static XDY_Coeff vm_Coef_b = 0.0;
static XDY_Coeff vm_Coef_c = 0.0;
static XDY_Coeff vm_Coef_d = 0.0;
static XDY_Coeff vm_Coef_e = 0.0;
static XDY_Coeff vm_Coef_f = 0.0;

/* declaration de fonctions internes */
int er_Calcul_RADT_Virtuelle(XDY_Eqt, XDY_Entier, XDY_Entier);

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Calcul des donnees glissantes 6mn
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Mesures_Glissantes(XDY_Horodate va_Horodate,
						  XDY_Eqt va_Num_RADT,
						  XDY_Entier va_Index,
						  XDY_Booleen va_Mes_Extrapolees)

/*
 * ARGUMENTS EN ENTREE :
 *   va_Horodate : Horodate
 *   va_Num_RADT : Numero Station RADT
 *   va_Index	: Index du tableau des valeurs
 *   va_Mes_Extrapolees : valeurs extrapolees ou non
 *
 * ARGUMENTS EN SORTIE :
 *
 *   pa_MsgTDC   : Message a envoyer a TETDC (equations d'alertes)
 *
 * CODE RETOUR :
 *   XDC_OK	succes
 *   XDC_NOK	echec
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *
 * Calcul des donnees glissantes 6mn :
 * 	Calcul des donnees glissantes par voie
 * 	Calcul des donnees glissantes par chaussee
 *	Envoi de XDM_EMES_6gl_Chaussee dans XDG_EMES_RADT_6mn_R<NomSite>
 *	Envoi de XDM_EMES_6gl dans XDG_EMES_RADT_6gl_R<NoEqt>
 *
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ : er_Mesures_Glissantes";

	ERDT_DONNEES_Chaussee *pl_ListeChaussee;
	ERDT_DONNEES_PtMes *pl_ListePtMes;
	XZSMT_Horodate pl_Horodatedecompo;
	XZSMT_Horodate vl_horodate_chaine;
	int vl_Horo_en_mn = 0;

	XDY_Datagroup pl_DG_gl_Ch;
	XDY_Datagroup pl_DG_gl_V;
	XDY_Eqt vl_Num_RADT = 0;
	XDY_Sens vl_Sens = 0;
	XDY_Entier vl_Chaussee = 0;
	XDY_Coeff vl_Q[ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS];
	XDY_Coeff vl_V[ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS];
	XDY_Coeff vl_TO[ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS];
	XDY_Coeff vl_PL[ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS];
	XDY_Entier vl_Tab_Index[7];
	XDY_Entier vl_Index = 0;
	XDY_Entier vl_i = 0;
	XDY_Entier vl_j = 0;
	XDY_Entier vl_k = 0;
	XDY_Entier vl_l = 0;
	XDY_Entier vl_Etat_Voie = XDC_VOIE_INCONNUE;
	XDY_Entier vl_Voie = 0;
	ldiv_t vl_Result_Div;

	XDY_Texte pl_Lig;
	XDY_FichEquext pl_PathFic;
	int vl_Fd = 0;
	XDY_Entier vl_retcode = 0;
	XDY_Entier vl_Nb_Param = 0;
	XDY_Booleen vl_Commentaire = XDC_FAUX;

	XDY_Entier vl_IndicChaussee[ERDTC_NB_VOIE_CHAUS];
	XDY_Entier vl_Glissantes_Q[ERDTC_NB_VOIE_CHAUS];
	XDY_Entier vl_Glissantes_V[ERDTC_NB_VOIE_CHAUS];
	XDY_Entier vl_Glissantes_TO[ERDTC_NB_VOIE_CHAUS];
	XDY_Entier vl_Glissantes_PL[ERDTC_NB_VOIE_CHAUS];
	ERDT_DONNEES_RADT *pl_ListeRADT;

	XDY_Entier vl_Nb_Voie_Inverse[9];

	XDY_Booleen vl_Validite = XDC_VRAI;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : er_Mesures_Glissantes Index = %d\tRADT = %d", va_Index, va_Num_RADT);

	/* Convertir l'horodate recue en une chaine puis en extraire l'heure */
	XZSM_11ConversionHorodate(va_Horodate, &pl_Horodatedecompo);
	strncpy(vl_horodate_chaine, pl_Horodatedecompo, sizeof(XZSMT_Horodate));
	vl_horodate_chaine[13] = '\0';
	vl_horodate_chaine[16] = '\0';
	vl_Horo_en_mn = (atoi(&vl_horodate_chaine[11]) * 60) + atoi(&vl_horodate_chaine[14]);

	for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
		if (pl_ListeRADT->ConfRADT.NumeroStation == va_Num_RADT)
			break;

	if ((vm_Coef_a == 0.0) && (vm_Coef_b == 0.0) && (vm_Coef_c == 0.0) && (vm_Coef_d == 0.0) && (vm_Coef_e == 0.0) && (vm_Coef_f == 0.0))
	{
		/*A Ouverture du fichier contenant les coefficients pour le calcul des donnees 6 mn glissantes */
		sprintf(pl_PathFic, "%s/%s", ERDTC_PATH_FIC_DFT, ERDTC_FIC_COEF_DFT);

		if ((vl_Fd = open(pl_PathFic, O_RDONLY)) == -1)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "er_Mesures_Glissantes : fichier %s. introuvable", ERDTC_FIC_COEF_DFT);
			XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : er_Mesures_Glissantes retourne XDC_NOK");
			return (XDC_NOK);
		}

		/*A Lecture de l'entete du fichier afin de connaitre la position du prochain enregistrement */
		while (ex_LireLigneFich(vl_Fd, pl_Lig, sizeof(pl_Lig)) != XZEXC_EOF)
		{
			/*B Verification que la ligne n'est pas un commentaire (Presence du caractere #) */
			for (vl_i = 0; vl_i < strlen(pl_Lig); vl_i++)
			{
				if (pl_Lig[vl_i] == ERDTC_COMMENTAIRE)
				{
					vl_Commentaire = XDC_VRAI;
					break;
				}
			}

			if (vl_Commentaire != XDC_VRAI)
			{
				/*B Extraction des valeurs des coefficients pour le calcul des donnees 6 mn glissantes */
				if ((vl_Nb_Param = sscanf(pl_Lig, "%f\t%f\t%f\t%f\t%f\t%f", &vm_Coef_a, &vm_Coef_b, &vm_Coef_c, &vm_Coef_d, &vm_Coef_e, &vm_Coef_f)) != 6)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "Pointeur fichier d'enregistrement des alarmes incorrect");
					return (XDC_NOK);
				}
			}
			vl_Commentaire = XDC_FAUX;
		}
		/*A Fermeture du fichier contenant les coefficients pour le calcul des donnees 6 mn glissantes */
		XZSS_13FermerFichier(vl_Fd);

		XZST_03EcritureTrace(XZSTC_INFO, "coef:%f\t%f\t%f\t%f\t%f\t%f ", vm_Coef_a, vm_Coef_b, vm_Coef_c, vm_Coef_d, vm_Coef_e, vm_Coef_f);
	}

	/*A Recherche des index du tableau de valeurs pour les minutes precedentes */
	vl_Tab_Index[0] = va_Index;

	for (vl_i = 0; vl_i < ERDTC_NB_INDEX_1MN; vl_i++)
	{
		vl_Result_Div = ldiv(vl_Tab_Index[vl_i], 6);

		if (vl_Result_Div.rem != 0)
			vl_Tab_Index[vl_i + 1] = vl_Tab_Index[vl_i] - 1;
		else
			vl_Tab_Index[vl_i + 1] = vl_Tab_Index[vl_i] + 5;
	}

	for (vl_Chaussee = 1; vl_Chaussee < ERDTC_NB_CHAUSSEE; vl_Chaussee++)
		for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
		{
			vl_Q[vl_Chaussee][vl_Voie] = (vl_Voie == 0) ? -1 : 0.0;
			vl_V[vl_Chaussee][vl_Voie] = (vl_Voie == 0) ? -1 : 0.0;
			vl_TO[vl_Chaussee][vl_Voie] = (vl_Voie == 0) ? -1 : 0.0;
			vl_PL[vl_Chaussee][vl_Voie] = (vl_Voie == 0) ? -1 : 0.0;
		}

	/*A Boucle sur le tableau de valeurs correspondant a la station RADT passee en argument
		en fonction des chaussees Sens1 et Sens 2 (uniquement) qui lui sont attribuees */
	for (vl_Chaussee = 1; vl_Chaussee < ERDTC_NB_CHAUSSEE; (vl_Chaussee == 4) ? vl_Chaussee += 3 : vl_Chaussee++)
	{
		/*A Calculs des donnees 6 mn glissantes Q, V et TO pour les voies 1,2,3,4 : */
		for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
		{
			if (pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie != XDC_VOIE_INCONNUE)
			{
				vl_Q[vl_Chaussee][vl_Voie] = 0;
				vl_V[vl_Chaussee][vl_Voie] = 0;
				vl_TO[vl_Chaussee][vl_Voie] = 0;
				vl_PL[vl_Chaussee][vl_Voie] = 0;

				for (vl_i = 0; vl_i <= ERDTC_NB_VOIE_CHAUS; vl_i++)
				{
					vl_Glissantes_Q[vl_Tab_Index[vl_i]] = XDC_RADT_Q_INCONNU;
					vl_Glissantes_V[vl_Tab_Index[vl_i]] = XDC_RADT_V_INCONNU;
					vl_Glissantes_TO[vl_Tab_Index[vl_i]] = XDC_RADT_TO_INCONNU;
					vl_Glissantes_PL[vl_Tab_Index[vl_i]] = XDC_RADT_PL_INCONNU;
				}

				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate = va_Horodate;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Sens = vl_Chaussee;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Periode = ERDTC_MINUTE;

				XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Ch = %d\tV = %d\t Ind = %d\tH = %lf\tS = %d\tP = %d", version, vl_Chaussee, vl_Voie, va_Index,
									 pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate,
									 pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Sens,
									 pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Periode);

				for (vl_j = ERDTC_NB_VOIE_CHAUS; vl_j >= 0; vl_j--)
				{
					if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_j]].Mesures.ValQ.Indic == XDC_RADT_INCONNU)
					{
						vl_l = 0;
						for (vl_k = vl_j; vl_k >= vl_l; vl_k--)
						{
							if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_k]].Mesures.ValQ.Indic != XDC_RADT_INCONNU)
							{
								vl_Glissantes_Q[vl_Tab_Index[vl_j]] = pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_k]].Mesures.ValQ.Val;
								vl_l = vl_k;
							}
						}
					}
					else
						vl_Glissantes_Q[vl_Tab_Index[vl_j]] = pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_j]].Mesures.ValQ.Val;
				}

				for (vl_j = 0; vl_j <= ERDTC_NB_VOIE_CHAUS; vl_j++)
					if (vl_Glissantes_Q[vl_Tab_Index[vl_j]] == XDC_RADT_Q_INCONNU)
					{
						vl_Validite = XDC_FAUX;
						break;
					}

				if (vl_Validite != XDC_FAUX)
					vl_Q[vl_Chaussee][vl_Voie] = (vm_Coef_a * vl_Glissantes_Q[vl_Tab_Index[0]]) +
												 (vm_Coef_b * vl_Glissantes_Q[vl_Tab_Index[1]]) +
												 (vm_Coef_c * vl_Glissantes_Q[vl_Tab_Index[2]]) +
												 (vm_Coef_d * vl_Glissantes_Q[vl_Tab_Index[3]]) +
												 (vm_Coef_e * vl_Glissantes_Q[vl_Tab_Index[4]]) +
												 (vm_Coef_f * vl_Glissantes_Q[vl_Tab_Index[5]]);
				else
					vl_Q[vl_Chaussee][vl_Voie] = -1;

				vl_Validite = XDC_VRAI;

				for (vl_j = ERDTC_NB_VOIE_CHAUS; vl_j >= 0; vl_j--)
				{
					if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_j]].Mesures.ValV.Indic == XDC_RADT_INCONNU)
					{
						vl_l = 0;
						for (vl_k = vl_j; vl_k >= vl_l; vl_k--)
						{
							if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_k]].Mesures.ValV.Indic != XDC_RADT_INCONNU)
							{
								vl_Glissantes_V[vl_Tab_Index[vl_j]] = pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_k]].Mesures.ValV.Val;
								vl_l = vl_k;
							}
						}
					}
					else
						vl_Glissantes_V[vl_Tab_Index[vl_j]] = pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_j]].Mesures.ValV.Val;
				}

				for (vl_j = 0; vl_j <= ERDTC_NB_VOIE_CHAUS; vl_j++)
					if (vl_Glissantes_V[vl_Tab_Index[vl_j]] == XDC_RADT_V_INCONNU)
					{
						vl_Validite = XDC_FAUX;
						break;
					}

				if (vl_Validite != XDC_FAUX)
					vl_V[vl_Chaussee][vl_Voie] = (vm_Coef_a * vl_Glissantes_V[vl_Tab_Index[0]]) +
												 (vm_Coef_b * vl_Glissantes_V[vl_Tab_Index[1]]) +
												 (vm_Coef_c * vl_Glissantes_V[vl_Tab_Index[2]]) +
												 (vm_Coef_d * vl_Glissantes_V[vl_Tab_Index[3]]) +
												 (vm_Coef_e * vl_Glissantes_V[vl_Tab_Index[4]]) +
												 (vm_Coef_f * vl_Glissantes_V[vl_Tab_Index[5]]);
				else
					vl_V[vl_Chaussee][vl_Voie] = -1;

				vl_Validite = XDC_VRAI;

				XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : Ch = %d\tV = %d\t Ind = %d\tV = %lf", version, vl_Chaussee, vl_Voie, va_Index, vl_V[vl_Chaussee][vl_Voie]);

				for (vl_j = ERDTC_NB_VOIE_CHAUS; vl_j >= 0; vl_j--)
				{
					if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_j]].Mesures.ValTO.Indic == XDC_RADT_INCONNU)
					{
						vl_l = 0;
						for (vl_k = vl_j; vl_k >= vl_l; vl_k--)
						{
							if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_k]].Mesures.ValTO.Indic != XDC_RADT_INCONNU)
							{
								vl_Glissantes_TO[vl_Tab_Index[vl_j]] = pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_k]].Mesures.ValTO.Val;
								vl_l = vl_k;
							}
						}
					}
					else
						vl_Glissantes_TO[vl_Tab_Index[vl_j]] = pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][vl_Tab_Index[vl_j]].Mesures.ValTO.Val;
				}

				for (vl_j = 0; vl_j <= ERDTC_NB_VOIE_CHAUS; vl_j++)
					if (vl_Glissantes_TO[vl_Tab_Index[vl_j]] == XDC_RADT_TO_INCONNU)
					{
						vl_Validite = XDC_FAUX;
						break;
					}

				if (vl_Validite != XDC_FAUX)
					vl_TO[vl_Chaussee][vl_Voie] = (vm_Coef_a * vl_Glissantes_TO[vl_Tab_Index[0]]) +
												  (vm_Coef_b * vl_Glissantes_TO[vl_Tab_Index[1]]) +
												  (vm_Coef_c * vl_Glissantes_TO[vl_Tab_Index[2]]) +
												  (vm_Coef_d * vl_Glissantes_TO[vl_Tab_Index[3]]) +
												  (vm_Coef_e * vl_Glissantes_TO[vl_Tab_Index[4]]) +
												  (vm_Coef_f * vl_Glissantes_TO[vl_Tab_Index[5]]);
				else
					vl_TO[vl_Chaussee][vl_Voie] = -1;

				vl_Validite = XDC_VRAI;

				if (pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic != XDC_RADT_INCONNU)
					vl_PL[vl_Chaussee][vl_Voie] = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val;
				else
					vl_PL[vl_Chaussee][vl_Voie] = -1;

				XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : Ch = %d\tV = %d\t Ind = %d\tTO = %lf", version, vl_Chaussee, vl_Voie, va_Index, vl_TO[vl_Chaussee][vl_Voie]);

				/*A Mise a jour du tableau des mesures 6mn glissantes par voie et par chaussee pour l'envoi a l'IHM */
				if ((vl_Q[vl_Chaussee][vl_Voie] >= 0) && (vl_Q[vl_Chaussee][vl_Voie] != -1))
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val = vl_Q[vl_Chaussee][vl_Voie];
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic =
						(pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic == XDC_RADT_RECONST) ? XDC_RADT_RECONST : XDC_RADT_CORRECT;
				}
				else
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
				}

				if ((vl_V[vl_Chaussee][vl_Voie] >= 0) && (vl_V[vl_Chaussee][vl_Voie] != -1))
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val = vl_V[vl_Chaussee][vl_Voie];
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic =
						(pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic == XDC_RADT_RECONST) ? XDC_RADT_RECONST : XDC_RADT_CORRECT;
				}
				else
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
				}

				if ((vl_TO[vl_Chaussee][vl_Voie] >= 0) && (vl_TO[vl_Chaussee][vl_Voie] != -1))
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val = vl_TO[vl_Chaussee][vl_Voie];
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic =
						(pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic == XDC_RADT_RECONST) ? XDC_RADT_RECONST : XDC_RADT_CORRECT;
				}
				else
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
				}

				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate = va_Horodate;

				/*B Envoi de XDM_EMES_6gl_Voie dans XDG_EMES_RADT_6GL_R_<NoEqt> si la voie existe */
				XZST_03EcritureTrace(XZSTC_INFO, "%s : Envoi du message XDM_EMES_6gl_Voie<NoEqt> en fournissant les valeur calculees", version);

				/*B Construction du DatGroup XDG_EMES_RADT_6GL_R_<NoEqtSensVoie> */
				sprintf(pl_DG_gl_V, "%s%d%d%d", XDG_EMES_RADT_6GL_R, va_Num_RADT, vl_Chaussee, vl_Voie);

				/*B Envoi du message XDM_EMES_6gl_Voie en fournissant les valeur recues */
				if (!TipcSrvMsgWrite(pl_DG_gl_V,
									 XDM_IdentMsg(XDM_EMES_6gl_Voie),
									 XDM_FLG_SRVMSGWRITE,
									 T_IPC_FT_REAL8, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Numero,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Periode,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Sens,
									 T_IPC_FT_INT2, vl_Voie,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic,
									 NULL))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EInit_IHM_6gl_Voie non effectue");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EInit_IHM_6gl_Voie effectue");
				}

				/*B Envoi du message */
				TipcSrvFlush();

				XZST_03EcritureTrace(XZSTC_INFO, "%s : Envoi message XDM_EMES_6gl_Voie effectue avec succes", version);
			}
		}
	}

	/*A Calculs des donnees 6 mn glissantes Q, V et TO pour la chaussee (voie 0) pour les Sens 1 et Sens 2 uniquement : */
	for (vl_Chaussee = (ERDTC_NB_CHAUSSEE - 1); vl_Chaussee > 0; (vl_Chaussee == 7) ? vl_Chaussee -= 3 : vl_Chaussee--)
	{
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate = va_Horodate;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Sens = vl_Chaussee;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Periode = ERDTC_MINUTE;

		if (pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Voie != XDC_VOIE_INCONNUE)
		{
			switch (vl_Chaussee)
			{
			/*B Cas de la chaussee Sens 1 */
			case 1:
				vl_Nb_Voie_Inverse[vl_Chaussee] = 0;
				for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					if (pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_NORMAL)
					{
						if (vl_Q[vl_Chaussee][vl_Voie] != -1)
							vl_Q[vl_Chaussee][0] = ((vl_Q[vl_Chaussee][0] == -1) ? 0 : vl_Q[vl_Chaussee][0]) + vl_Q[vl_Chaussee][vl_Voie];
						if (vl_V[vl_Chaussee][vl_Voie] != -1)
							if (vl_V[vl_Chaussee][vl_Voie] > 0.0)
								vl_V[vl_Chaussee][0] = ((vl_V[vl_Chaussee][0] == -1) ? 0 : vl_V[vl_Chaussee][0]) + (float)(vl_Q[vl_Chaussee][vl_Voie] / vl_V[vl_Chaussee][vl_Voie]);
							else
								vl_Nb_Voie_Inverse[vl_Chaussee]++;

						if (vl_TO[vl_Chaussee][vl_Voie] != -1)
							vl_TO[vl_Chaussee][0] = ((vl_TO[vl_Chaussee][0] == -1) ? 0 : vl_TO[vl_Chaussee][0]) + vl_TO[vl_Chaussee][vl_Voie];
						if (vl_PL[vl_Chaussee][vl_Voie] != -1)
							/*modif hilmarcher
							vl_PL[vl_Chaussee][0] = ((vl_PL[vl_Chaussee][0] == -1) ? 0 : vl_PL[vl_Chaussee][0]) + vl_PL[vl_Chaussee][vl_Voie];
							fin modif */
							vl_PL[vl_Chaussee][0] = ((vl_PL[vl_Chaussee][0] == -1) ? 0 : vl_PL[vl_Chaussee][0]) +
													((float)vl_PL[vl_Chaussee][vl_Voie] * (float)vl_Q[vl_Chaussee][vl_Voie]);
					}
					if (pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_INVERSE)
					{
						vl_Nb_Voie_Inverse[vl_Chaussee]++;
						if (vl_Q[vl_Chaussee][vl_Voie] != -1)
							vl_Q[vl_Chaussee + 1][0] = ((vl_Q[vl_Chaussee + 1][0] == -1) ? 0 : vl_Q[vl_Chaussee + 1][0]) + vl_Q[vl_Chaussee][vl_Voie];
						if (vl_V[vl_Chaussee][vl_Voie] != -1)
							if (vl_V[vl_Chaussee][vl_Voie] > 0.0)
								vl_V[vl_Chaussee + 1][0] = ((vl_V[vl_Chaussee + 1][0] == -1) ? 0 : vl_V[vl_Chaussee + 1][0]) + (float)(vl_Q[vl_Chaussee][vl_Voie] / vl_V[vl_Chaussee][vl_Voie]);

						if (vl_TO[vl_Chaussee][vl_Voie] != -1)
							vl_TO[vl_Chaussee + 1][0] = ((vl_TO[vl_Chaussee + 1][0] == -1) ? 0 : vl_TO[vl_Chaussee + 1][0]) + vl_TO[vl_Chaussee][vl_Voie];
						if (vl_PL[vl_Chaussee][vl_Voie] != -1)
							/*modif hilmarcher
							vl_PL[vl_Chaussee + 1][0] = ((vl_PL[vl_Chaussee+1][0] == -1) ? 0 : vl_PL[vl_Chaussee + 1][0]) + vl_PL[vl_Chaussee][vl_Voie];
						 fin modif*/
							vl_PL[vl_Chaussee + 1][0] = ((vl_PL[vl_Chaussee + 1][0] == -1) ? 0 : vl_PL[vl_Chaussee + 1][0]) +
														((float)vl_PL[vl_Chaussee][vl_Voie] * (float)vl_Q[vl_Chaussee][vl_Voie]);
					}
					if ((pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_INVERSE) ||
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_BLOQUEE) ||
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_NEUTRALISEE) ||
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_BLOQUEE_NEUTRALISEE))
						vl_Nb_Voie_Inverse[vl_Chaussee]++;
				}
				vl_PL[vl_Chaussee][0] = (vl_PL[vl_Chaussee][0] / vl_Q[vl_Chaussee][0]);
				vl_PL[vl_Chaussee + 1][0] = (vl_PL[vl_Chaussee + 1][0] / vl_Q[vl_Chaussee + 1][0]);
				break;

			/*B Cas de la chaussee Sens 2 */
			case 2:
				vl_Nb_Voie_Inverse[vl_Chaussee] = 0;
				for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					if (pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_NORMAL)
					{
						if (vl_Q[vl_Chaussee][vl_Voie] != -1)
							vl_Q[vl_Chaussee][0] = ((vl_Q[vl_Chaussee][0] == -1) ? 0 : vl_Q[vl_Chaussee][0]) + vl_Q[vl_Chaussee][vl_Voie];
						if (vl_V[vl_Chaussee][vl_Voie] != -1)
							if (vl_V[vl_Chaussee][vl_Voie] > 0.0)
								vl_V[vl_Chaussee][0] = ((vl_V[vl_Chaussee][0] == -1) ? 0 : vl_V[vl_Chaussee][0]) + (float)(vl_Q[vl_Chaussee][vl_Voie] / vl_V[vl_Chaussee][vl_Voie]);
							else
								vl_Nb_Voie_Inverse[vl_Chaussee]++;
						if (vl_TO[vl_Chaussee][vl_Voie] != -1)
							vl_TO[vl_Chaussee][0] = ((vl_TO[vl_Chaussee][0] == -1) ? 0 : vl_TO[vl_Chaussee][0]) + vl_TO[vl_Chaussee][vl_Voie];
						if (vl_PL[vl_Chaussee][vl_Voie] != -1)
							vl_PL[vl_Chaussee][0] = ((vl_PL[vl_Chaussee][0] == -1) ? 0 : vl_PL[vl_Chaussee][0]) + vl_PL[vl_Chaussee][vl_Voie];
					}

					if (pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_INVERSE)
					{
						vl_Nb_Voie_Inverse[vl_Chaussee]++;
						if (vl_Q[vl_Chaussee][vl_Voie] != -1)
							vl_Q[vl_Chaussee - 1][0] = ((vl_Q[vl_Chaussee - 1][0] == -1) ? 0 : vl_Q[vl_Chaussee - 1][0]) + vl_Q[vl_Chaussee][vl_Voie];
						if (vl_V[vl_Chaussee][vl_Voie] != -1)
							if (vl_V[vl_Chaussee][vl_Voie] > 0.0)
								vl_V[vl_Chaussee - 1][0] = ((vl_V[vl_Chaussee - 1][0] == -1) ? 0 : vl_V[vl_Chaussee - 1][0]) + (float)(vl_Q[vl_Chaussee][vl_Voie] / vl_V[vl_Chaussee][vl_Voie]);
							else
								vl_Nb_Voie_Inverse[vl_Chaussee]++;

						if (vl_TO[vl_Chaussee][vl_Voie] != -1)
							vl_TO[vl_Chaussee - 1][0] = ((vl_TO[vl_Chaussee - 1][0] == -1) ? 0 : vl_TO[vl_Chaussee - 1][0]) + vl_TO[vl_Chaussee][vl_Voie];
						if (vl_PL[vl_Chaussee][vl_Voie] != -1)
							vl_PL[vl_Chaussee - 1][0] = ((vl_PL[vl_Chaussee - 1][0] == -1) ? 0 : vl_PL[vl_Chaussee - 1][0]) + vl_PL[vl_Chaussee][vl_Voie];
					}

					if ((pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_INVERSE) ||
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_BLOQUEE) ||
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_NEUTRALISEE) ||
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_BLOQUEE_NEUTRALISEE))
						vl_Nb_Voie_Inverse[vl_Chaussee]++;
				}
				break;

			/*B Autres Cas */
			default:
				vl_Nb_Voie_Inverse[vl_Chaussee] = 0;
				for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					if ((pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_NORMAL) ||
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_INVERSE))
					{
						if (vl_Q[vl_Chaussee][vl_Voie] != -1)
							vl_Q[vl_Chaussee][0] = ((vl_Q[vl_Chaussee][0] == -1) ? 0 : vl_Q[vl_Chaussee][0]) + vl_Q[vl_Chaussee][vl_Voie];
						vl_V[vl_Chaussee][0] = -1;
						if (vl_TO[vl_Chaussee][vl_Voie] != -1)
							vl_TO[vl_Chaussee][0] = ((vl_TO[vl_Chaussee][0] == -1) ? 0 : vl_TO[vl_Chaussee][0]) + vl_TO[vl_Chaussee][vl_Voie];
						vl_PL[vl_Chaussee][0] = -1;
					}
				}
				break;

			} /* fin switch */

		} /* fin if */
	}	  /* fin for */

	for (vl_Chaussee = (ERDTC_NB_CHAUSSEE - 1); vl_Chaussee > 0; (vl_Chaussee == 7) ? vl_Chaussee -= 3 : vl_Chaussee--)
	{
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate = va_Horodate;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Sens = vl_Chaussee;
		pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Periode = ERDTC_MINUTE;

		if (pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Voie != XDC_VOIE_INCONNUE)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : CHAUSSEE=%d\tV = %lf\tQ = %lf\t TO = %lf\tPL = %lf",
								 version, vl_Chaussee, vl_V[vl_Chaussee][0], vl_Q[vl_Chaussee][0], vl_TO[vl_Chaussee][0], vl_PL[vl_Chaussee][0]);
			/*A Mise a jour du tableau des mesures 6mn glissantes pour chaque chaussee pour l'envoi a l'IHM */
			if (vl_V[vl_Chaussee][0] > 0.0)
			{
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = (float)(vl_Q[vl_Chaussee][0] / vl_V[vl_Chaussee][0]);
				/* DEM/1015 */
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_CORRECT;
				for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
					if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic == XDC_RADT_RECONST)
					{
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_RECONST;
						break;
					}
			}
			else
			{
				if (vl_V[vl_Chaussee][0] == 0.0)
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = 0;
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_CORRECT;
				}
				else
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
				}
			}

			if (vl_Q[vl_Chaussee][0] >= 0.0)
			{

				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = vl_Q[vl_Chaussee][0];
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_CORRECT;
				for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
					if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic == XDC_RADT_RECONST)
					{
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_RECONST;
						break;
					}
			}
			else
			{
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
			}

			if (vl_TO[vl_Chaussee][0] >= 0.0)
			{
				if ((pg_Nb_Voie[va_Num_RADT][vl_Chaussee] - vl_Nb_Voie_Inverse[vl_Chaussee]) > 0)
				{
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = (float)(vl_TO[vl_Chaussee][0] / (pg_Nb_Voie[va_Num_RADT][vl_Chaussee] - vl_Nb_Voie_Inverse[vl_Chaussee]));
					pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_CORRECT;
					for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
						if (pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic == XDC_RADT_RECONST)
						{
							pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_RECONST;
							break;
						}
				}
			}
			else
			{
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
				pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
			}

			pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate = va_Horodate;

			/* DEM/681 Si entre 07:30 et 22:00 l'une des voies non inhibees dans le sens 1 ou 2 */
			/*   a une quantite negative ou nulle alors mettre toutes les valeurs chaussee a -1 */
			if (((vl_Chaussee == 1) || (vl_Chaussee == 2)) && ((vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) && (vl_Horo_en_mn < pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur)))
			{
				for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					if ((pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_NORMAL) &&
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val <= 0) &&
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic != XDC_RADT_INCONNU))
					{
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = -1;
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = -1;
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Val = -1;
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
						break;
					}
				}
			}

			/*B Envoi de XDM_EMES_6gl_Chaussee dans XDG_EMES_RADT_6GL_R_<NomSite> */
			XZST_03EcritureTrace(XZSTC_INFO, "%s : Envoi du message XDM_EMES_6gl_Chaussee en fournissant les valeur calculees", version);

			/*sprintf ( pl_DG_gl_Ch, "%s_%s", XDG_EMES_RADT_6GL_R, vg_NomSite ); DEM748*/
			sprintf(pl_DG_gl_Ch, "%s_%s", XDG_EMES_RADT_6GL_R, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion));
			XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Envoi %s", version, pl_DG_gl_Ch);

			/*B Envoi du message XDM_EMES_6gl_Chaussee en fournissant les valeur recues */
			if (!TipcSrvMsgWrite(pl_DG_gl_Ch,
								 XDM_IdentMsg(XDM_EMES_6gl_Chaussee),
								 XDM_FLG_SRVMSGWRITE,
								 T_IPC_FT_REAL8, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate,
								 T_IPC_FT_STR, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion),
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Numero,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Periode,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Sens,
								 T_IPC_FT_INT2, XDC_RADT_CHAUSSEE,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Val,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Indic,
								 NULL))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EInit_IHM_6gl_Chaussee non effectue");
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EInit_IHM_6gl_Chaussee effectue");
				if (va_Index == 0)
					TipcSrvMsgWrite(XDG_EMES_RADT_6GL_R,
									XDM_IdentMsg(XDM_EMES_6gl_Chaussee),
									XDM_FLG_SRVMSGWRITE,
									T_IPC_FT_REAL8, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate,
									T_IPC_FT_STR, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion),
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Numero,
									T_IPC_FT_INT2, ERDTC_6MINUTE,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Sens,
									T_IPC_FT_INT2, XDC_RADT_CHAUSSEE,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Val,
									T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Indic,
									NULL);
			}

			/*B Envoi du message */
			TipcSrvFlush();

			XZST_03EcritureTrace(XZSTC_INFO, "%s : Envoi message XDM_EMES_6gl_Chaussee effectue avec succes\n", version);

			sprintf(pl_DG_gl_Ch, "%s%d%d%d", XDG_EMES_RADT_6GL_R, va_Num_RADT, vl_Chaussee, XDC_RADT_CHAUSSEE);

			/*B Envoi du message XDM_EMES_6gl_Voie en fournissant les valeur recues */
			if (!TipcSrvMsgWrite(pl_DG_gl_Ch,
								 XDM_IdentMsg(XDM_EMES_6gl_Voie),
								 XDM_FLG_SRVMSGWRITE,
								 T_IPC_FT_REAL8, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Numero,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Periode,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Sens,
								 T_IPC_FT_INT2, XDC_RADT_CHAUSSEE,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Val,
								 T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Indic,
								 NULL))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EMES_6gl_Voie non effectue");
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EMES_6gl_Voie effectue");
			}

			/*B Envoi du message */
			TipcSrvFlush();
			for (vl_Voie = 0; vl_Voie < 4; vl_Voie++)
				XZST_03EcritureTrace(XZSTC_INFO, "6mn_gl pour chausse = %d; pour voie %d = %d", vl_Chaussee, vl_Voie, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);

			XZST_03EcritureTrace(XZSTC_INFO, "%s : Envoi message XDM_EMES_6mn_Chaussee effectue avec succes\n", version);
		} /* fin if */
	}	  /* fin for */

	/* Envoi donnees inconnues pour voie neutralise */
	for (pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant)
	{
		if ((pl_ListePtMes->ConfPtMes.NumeroStation == va_Num_RADT) &&
			(pl_ListePtMes->EtatVoie != XDC_VOIE_SENS_NORMAL) && (pl_ListePtMes->EtatVoie != XDC_VOIE_SENS_INVERSE))
		{
			/*B Construction du DatGroup XDG_EMES_RADT_6GL_R_<NoEqt> */
			sprintf(pl_DG_gl_V, "%s%d%d%d", XDG_EMES_RADT_6GL_R, va_Num_RADT, pl_ListePtMes->ConfPtMes.Sens, pl_ListePtMes->ConfPtMes.Voie);

			/*B Envoi du message XDM_EMES_6gl_Voie en fournissant les valeur recues */
			if (!TipcSrvMsgWrite(pl_DG_gl_V,
								 XDM_IdentMsg(XDM_EMES_6gl_Voie),
								 XDM_FLG_SRVMSGWRITE,
								 T_IPC_FT_REAL8, va_Horodate,
								 T_IPC_FT_INT2, va_Num_RADT,
								 T_IPC_FT_INT2, ERDTC_MINUTE,
								 T_IPC_FT_INT2, pl_ListePtMes->ConfPtMes.Sens,
								 T_IPC_FT_INT2, pl_ListePtMes->ConfPtMes.Voie,
								 T_IPC_FT_INT2, XDC_RADT_Q_INCONNU,
								 T_IPC_FT_INT2, XDC_RADT_INCONNU,
								 T_IPC_FT_INT2, XDC_RADT_V_INCONNU,
								 T_IPC_FT_INT2, XDC_RADT_INCONNU,
								 T_IPC_FT_INT2, XDC_RADT_TO_INCONNU,
								 T_IPC_FT_INT2, XDC_RADT_INCONNU,
								 T_IPC_FT_INT2, XDC_RADT_PL_INCONNU,
								 T_IPC_FT_INT2, XDC_RADT_INCONNU,
								 NULL))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EMES_6gl_Voie neutralise non effectue");
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EMES_6gl_Voie neutralise effectue sur %s", pl_DG_gl_V); /*linux*/
			}
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne OK", version);
	/* Si la station possede une chaussee virtuelle appel de er_Calcul_RADT_Virtuelle */
	for (vl_i = 0; vl_i < ERDTC_NUM_MAX_STATION; vl_i++)
	{
		/*		XZST_03EcritureTrace( XZSTC_DEBUG1,"VIRT0(%d) %d %d %d",
					va_Num_RADT,
					pg_Tab_RADT_Virtuelle[vl_i].NumStation1,
					pg_Tab_RADT_Virtuelle[vl_i].NumStation2,
					pg_Tab_RADT_Virtuelle[vl_i].NumStation3); */
		if (pg_Tab_RADT_Virtuelle[vl_i].NumStation1 == 0)
			continue;

		if ((pg_Tab_RADT_Virtuelle[vl_i].NumStation1 == va_Num_RADT) ||
			(pg_Tab_RADT_Virtuelle[vl_i].NumStation2 == va_Num_RADT) ||
			(pg_Tab_RADT_Virtuelle[vl_i].NumStation3 == va_Num_RADT))
		{
			/*
					XZST_03EcritureTrace( XZSTC_DEBUG1,"VIRT(%d) %d %d %d",
						va_Num_RADT,
						pg_Tab_RADT_Virtuelle[vl_i].NumStation1,
						pg_Tab_RADT_Virtuelle[vl_i].NumStation2,
						pg_Tab_RADT_Virtuelle[vl_i].NumStation3); */

			er_Calcul_RADT_Virtuelle(va_Num_RADT, ERDTC_MINUTE, va_Index);
			break;
		}
	}

	/* Si station possede une chaussee virtuelle */
	/*if ( ( pg_Tab_RADT_Virtuelle[pg_Tab_RADT[va_Num_RADT]].NumStation1 > 0 ) ||
			( pg_Tab_RADT_Virtuelle[pg_Tab_RADT[va_Num_RADT]].NumStation2 > 0 ) ||
			( pg_Tab_RADT_Virtuelle[pg_Tab_RADT[va_Num_RADT]].NumStation3 > 0 ) )
		er_Calcul_RADT_Virtuelle( va_Num_RADT, ERDTC_6MINUTE, va_Index );*/
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *
 *  Calcul extrapolation des donnees manquantes
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int er_Extrapolation(XDY_Entier va_Index,
					 XDY_Entier va_Index_Prec,
					 XDY_Horodate va_Horodate,
					 XDY_Eqt va_Num_RADT,
					 XDY_Sens va_Chaussee,
					 XDY_Voie va_Num_Voie,
					 XDY_Entier va_Freq_Mes,
					 XDY_Entier va_Type_Mes,
					 XDY_Mot va_TypeStation)

/*
 * ARGUMENTS EN ENTREE :
 *
 *   va_Index		: Index de stockage des valeurs
 *   va_Index_Prec	: Index Precedent de stockage des valeurs
 *   va_Horodate		: Horodate d'extrapolation
 *   va_Num_RADT 	: Numero de la station RADT
 *   va_Chaussee 	: Sens
 *   va_Num_Voie		: Voie a extrapole
 *   va_Freq_Mes		: Frequence des donnees a extrapolees (ERDTC_MINUTE ou ERDTC_6MINUTE )
 *   va_Type_Mes		: Type de donnees a extrapolees (ERDTC_PL:PL ou ERDTC_NON_PL:QVTO )
 *
 * ARGUMENTS EN SORTIE :
 *
 * CODE RETOUR :
 *   XDC_OK	succes
 *   XDC_NOK	echec
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *
 * Extrapolation des mesures en fonction de l'historique de l'heure precedente, des donnees des voies adjacentes et des
 * station amont et aval et des coefficients de calcul
 *
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ : er_Extrapolation";

	ERDT_DONNEES_RADT *pl_ListeRADT;
	ERDT_EMes_RADT *pl_ERDT_Mes = NULL;
	ERDT_EMes_RADT *pl_Mes_Am = NULL;
	ERDT_EMes_RADT *pl_Mes_Av = NULL;
	XDY_Horodate vl_Horodate = 0.0;
	XDY_Eqt vl_Num_RADT = 0;
	XDY_Eqt vl_Num_Station = 0;
	XDY_Voie vl_Chaussee = 0;
	XDY_Entier vl_Index = 0;

	float vl_Qadj = 0.0;
	float vl_Vadj = 0.0;
	float vl_TOadj = 0.0;
	float vl_PLadj = 0.0;

	int vl_Q[ERDTC_NB_VOIE_CHAUS];
	int vl_V[ERDTC_NB_VOIE_CHAUS];
	int vl_TO[ERDTC_NB_VOIE_CHAUS];
	int vl_PL[ERDTC_NB_VOIE_CHAUS];

	XDY_Moyenne vl_Am = 0;
	XDY_Moyenne vl_Av = 0;

	int vl_Nb_Q[ERDTC_NB_VOIE_CHAUS];
	int vl_Nb_V[ERDTC_NB_VOIE_CHAUS];
	int vl_Nb_TO[ERDTC_NB_VOIE_CHAUS];
	int vl_Nb_PL[ERDTC_NB_VOIE_CHAUS];

	XDY_Moyenne vl_V_Moy[ERDTC_NB_VOIE_CHAUS];
	XDY_Moyenne vl_Q_Moy[ERDTC_NB_VOIE_CHAUS];
	XDY_Moyenne vl_TO_Moy[ERDTC_NB_VOIE_CHAUS];
	XDY_Moyenne vl_PL_Moy[ERDTC_NB_VOIE_CHAUS];

	int vl_div = 0;

	XDY_Moyenne vl_Pct_V[ERDTC_NB_VOIE_CHAUS];
	XDY_Moyenne vl_Pct_Q[ERDTC_NB_VOIE_CHAUS];
	XDY_Moyenne vl_Pct_TO[ERDTC_NB_VOIE_CHAUS];
	XDY_Moyenne vl_Pct_PL[ERDTC_NB_VOIE_CHAUS];

	int vl_Nb_Pct = 0;
	int vl_Nb_PctAm = 0;
	int vl_Nb_PctAv = 0;

	int vl_i = 0;
	int vl_j = 0;
	int vl_Sens = 0;
	int vl_Voie = 0;
	int vl_Cpt_Defaut = 0;
	ldiv_t vl_Result_Div;

	XDY_Eqt vl_Num_Station_Amont[2];
	XDY_Eqt vl_Num_Station_Aval[2];

	int vl_Index_Station_Amont[2];
	int vl_Index_Station_Aval[2];
	int vl_Extra = 0;
	int vl_Calcul_coef = 0;
	FILE *pl_Fd_Am = NULL;
	FILE *pl_Fd_Av = NULL;
	char pl_PathEtFic[96] = "";

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : er_Extrapolation Index = %d  Index_Prec = %d  RADT = %d  Chaus. = %d  Voie = %d  Freq = %d  Type = %d  Type ST = %d",
						 va_Index, va_Index_Prec, va_Num_RADT, va_Chaussee, va_Num_Voie,
						 va_Freq_Mes, va_Type_Mes, va_TypeStation);
	/*A Initialilisation des tableaux */

	for (vl_i = 0; vl_i < ERDTC_NB_VOIE_CHAUS; vl_i++)
	{
		vl_Q[vl_i] = 0;
		vl_V[vl_i] = 0;
		vl_TO[vl_i] = 0;
		vl_PL[vl_i] = 0;

		vl_V_Moy[vl_i] = 0.0;
		vl_Q_Moy[vl_i] = 0.0;
		vl_TO_Moy[vl_i] = 0.0;
		vl_PL_Moy[vl_i] = 0.0;

		vl_Nb_Q[vl_i] = 0;
		vl_Nb_V[vl_i] = 0;
		vl_Nb_TO[vl_i] = 0;
		vl_Nb_PL[vl_i] = 0;

		vl_Pct_V[vl_i] = 0.0;
		vl_Pct_Q[vl_i] = 0.0;
		vl_Pct_TO[vl_i] = 0.0;
		vl_Pct_PL[vl_i] = 0.0;
	}

	/*A S'il n'existe qu'un seul defaut de mesures => Extrapolation en fonction des voies adjacentes */
	if (va_Chaussee != ERDTC_TOUS_SENS && va_Num_Voie != ERDTC_TOUTES_VOIES)
	{
		/*B Calcul de la somme de Q, V et TO par voie de la chaussee */
		for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
		{
			for (vl_i = 1; vl_i < ERDTC_NB_INDEX_6MN; vl_i++)
			{
				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][vl_i].Mesures.ValQ.Indic != XDC_RADT_INCONNU)
				{
					vl_Q[vl_Voie] += pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][vl_i].Mesures.ValQ.Val;
					vl_Nb_Q[vl_Voie]++;
				}

				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][vl_i].Mesures.ValV.Indic != XDC_RADT_INCONNU)
				{
					vl_V[vl_Voie] += pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][vl_i].Mesures.ValV.Val;
					vl_Nb_V[vl_Voie]++;
				}

				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][vl_i].Mesures.ValTO.Indic != XDC_RADT_INCONNU)
				{
					vl_TO[vl_Voie] += pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][vl_i].Mesures.ValTO.Val;
					vl_Nb_TO[vl_Voie]++;
				}

				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][vl_i].Mesures.ValPL.Indic != XDC_RADT_INCONNU)
				{
					vl_PL[vl_Voie] += pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][vl_i].Mesures.ValPL.Val;
					vl_Nb_PL[vl_Voie]++;
				}
			}
		}

		/*B On deduit la moyenne de Q, V et TO par voie de la chaussee => Calcul du Pourcentage de Q, V et TO par voie de la chaussee */
		for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
		{
			if (vl_Nb_Q[vl_Voie] > 0 && vl_Q_Moy[vl_Voie] != 0)
			{
				vl_Q_Moy[vl_Voie] = vl_Q[vl_Voie] / vl_Nb_Q[vl_Voie];
				vl_Pct_Q[vl_Voie] = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val / vl_Q_Moy[vl_Voie];
			}
			else
				vl_Q_Moy[vl_Voie] = ERDTC_MES_INC;

			if (vl_Nb_V[vl_Voie] > 0 && vl_V_Moy[vl_Voie] != 0)
			{
				vl_V_Moy[vl_Voie] = vl_V[vl_Voie] / vl_Nb_V[vl_Voie];
				vl_Pct_V[vl_Voie] = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val / vl_V_Moy[vl_Voie];
			}
			else
				vl_V_Moy[vl_Voie] = ERDTC_MES_INC;

			if (vl_Nb_TO[vl_Voie] > 0 && vl_TO_Moy[vl_Voie] != 0)
			{
				vl_TO_Moy[vl_Voie] = vl_TO[vl_Voie] / vl_Nb_TO[vl_Voie];
				vl_Pct_TO[vl_Voie] = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val / vl_TO_Moy[vl_Voie];
			}
			else
				vl_TO_Moy[vl_Voie] = ERDTC_MES_INC;

			if (vl_Nb_PL[vl_Voie] > 0 && vl_PL_Moy[vl_Voie] != 0)
			{
				vl_PL_Moy[vl_Voie] = (float)vl_PL[vl_Voie] / vl_Nb_PL[vl_Voie];
				vl_Pct_PL[vl_Voie] = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val / (float)vl_PL_Moy[vl_Voie];
			}
			else
				vl_PL_Moy[vl_Voie] = ERDTC_MES_INC;
		}

		/*B Calcul du pourcentage des donnees de Q, V et TO des voies adjacentes de la chaussee */
		for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
		{
			if (vl_Voie != va_Num_Voie)
			{
				if (vl_Pct_Q[vl_Voie] != ERDTC_MES_INC &&
					vl_Pct_V[vl_Voie] != ERDTC_MES_INC &&
					vl_Pct_TO[vl_Voie] != ERDTC_MES_INC &&
					vl_Pct_PL[vl_Voie] != ERDTC_MES_INC)
				{
					vl_Qadj += vl_Pct_Q[vl_Voie];
					vl_Vadj += vl_Pct_V[vl_Voie];
					vl_TOadj += vl_Pct_TO[vl_Voie];
					vl_PLadj += vl_Pct_PL[vl_Voie];
					vl_Nb_Pct++;
				}
			}
		}

		XZST_03EcritureTrace(XZSTC_DEBUG3, " %s : Sens = %d  Voie = %d  vl_Vadj = %lf  vl_Qadj = %lf  vl_TOadj = %lf  vl_PLadj = %lf  vl_Nb_Pct = %lf", version,
							 va_Chaussee,
							 vl_Voie,
							 vl_Vadj,
							 vl_Qadj,
							 vl_TOadj,
							 vl_PLadj,
							 vl_Nb_Pct);

		/*B On deduit les valeurs a extrapolees pour la voie passee en parametre */
		switch (va_Freq_Mes)
		{
		case ERDTC_MINUTE:
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Horodate = va_Horodate;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValQ.Val = (int)((vl_Qadj / vl_Nb_Pct) * vl_Q_Moy[va_Num_Voie]);
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValQ.Indic = XDC_RADT_RECONST;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValV.Val = (int)((vl_Vadj / vl_Nb_Pct) * vl_V_Moy[va_Num_Voie]);
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValV.Indic = XDC_RADT_RECONST;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValTO.Val = (int)((vl_TOadj / vl_Nb_Pct) * vl_TO_Moy[va_Num_Voie]);
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValTO.Indic = XDC_RADT_RECONST;
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValPL.Val = (float)((vl_PLadj / vl_Nb_Pct) * vl_PL_Moy[va_Num_Voie]);
			pg_ERDT_Mes_1mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValPL.Indic = XDC_RADT_RECONST;

			pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Horodate = va_Horodate;
			pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValPL.Val = (float)((vl_PLadj / vl_Nb_Pct) * vl_PL_Moy[va_Num_Voie]);
			pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValPL.Indic = XDC_RADT_RECONST;
			break;

		case ERDTC_6MINUTE:
			switch (va_Type_Mes)
			{
			case ERDTC_NON_PL:
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Horodate = va_Horodate;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValQ.Val = (int)((vl_Qadj / vl_Nb_Pct) * vl_Q_Moy[va_Num_Voie]);
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValQ.Indic = XDC_RADT_RECONST;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValV.Val = (int)((vl_Vadj / vl_Nb_Pct) * vl_V_Moy[va_Num_Voie]);
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValV.Indic = XDC_RADT_RECONST;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValTO.Val = (int)((vl_TOadj / vl_Nb_Pct) * vl_TO_Moy[va_Num_Voie]);
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValTO.Indic = XDC_RADT_RECONST;
				break;

			case ERDTC_PL:
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Horodate = va_Horodate;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValPL.Val = (float)((vl_PLadj / vl_Nb_Pct) * (float)vl_PL_Moy[va_Num_Voie]);
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValPL.Indic = XDC_RADT_RECONST;
				break;
			}
		}

		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : Sortie avec valeurs XDC_RADT_RECONST des voies adjacentes de la chaussee de la station %d", va_Num_RADT);
		XZST_03EcritureTrace(XZSTC_DEBUG3, " valPL_1mn=%d, valPL_6mn=%d",
							 pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValPL.Val,
							 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][va_Chaussee][va_Num_Voie][va_Index].Mesures.ValPL.Val);

		return (XDC_OK);
	}
	else

	/*A S'il existe plus d'un defaut de mesures sur la chaussee => Extrapolation en fonction des station Amont et Aval */
	{
		/*B Recherche des stations Amont et Aval de la station RADT passee en argument */
		for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
			if (pl_ListeRADT->ConfRADT.NumeroStation == va_Num_RADT)
			{
				vl_Num_Station_Amont[0] = pl_ListeRADT->ConfRADT.NumStationAmontS1;
				vl_Num_Station_Amont[1] = pl_ListeRADT->ConfRADT.NumStationAmontS2;
				vl_Num_Station_Aval[0] = pl_ListeRADT->ConfRADT.NumStationAvalS1;
				vl_Num_Station_Aval[1] = pl_ListeRADT->ConfRADT.NumStationAvalS2;
				XZST_03EcritureTrace(XZSTC_DEBUG1, "er_Extrapolation : AmS1 = %d :: AmS2 = %d :: AvS1 = %d :: AvS2 = %d", vl_Num_Station_Amont[0],
									 vl_Num_Station_Amont[1], vl_Num_Station_Aval[0], vl_Num_Station_Aval[1]);
				XZST_03EcritureTrace(XZSTC_DEBUG1, "er_Extrapolation : AmS1 = %d :: AmS2 = %d :: AvS1 = %d :: AvS2 = %d",
									 pg_Tab_RADT[vl_Num_Station_Amont[0]],
									 pg_Tab_RADT[vl_Num_Station_Amont[1]],
									 pg_Tab_RADT[vl_Num_Station_Aval[0]],
									 pg_Tab_RADT[vl_Num_Station_Aval[1]]);

				vl_Num_Station_Amont[0] = ((vl_Index_Station_Amont[0] = er_mesures_valides(vl_Num_Station_Amont[0], XDC_SENS_SUD)) >= 0) ? vl_Num_Station_Amont[0] : 0;
				vl_Num_Station_Amont[1] = ((vl_Index_Station_Amont[1] = er_mesures_valides(vl_Num_Station_Amont[1], XDC_SENS_NORD)) >= 0) ? vl_Num_Station_Amont[1] : 0;
				vl_Num_Station_Aval[0] = ((vl_Index_Station_Aval[0] = er_mesures_valides(vl_Num_Station_Aval[0], XDC_SENS_SUD)) >= 0) ? vl_Num_Station_Aval[0] : 0;
				vl_Num_Station_Aval[1] = ((vl_Index_Station_Aval[1] = er_mesures_valides(vl_Num_Station_Aval[1], XDC_SENS_NORD)) >= 0) ? vl_Num_Station_Aval[1] : 0;
				XZST_03EcritureTrace(XZSTC_DEBUG1, "er_Extrapolation : Apres verif mesures valides  AmS1 = %d i=%d :: AmS2 = %d i=%d :: AvS1 = %d i=%d :: AvS2 = %d i=%d ",
									 vl_Num_Station_Amont[0], vl_Index_Station_Amont[0], vl_Num_Station_Amont[1], vl_Index_Station_Amont[1],
									 vl_Num_Station_Aval[0], vl_Index_Station_Aval[0], vl_Num_Station_Aval[1], vl_Index_Station_Aval[1]);
				break;
			}

		vl_Num_RADT = pg_Tab_RADT[va_Num_RADT];
		/*A RAZ des tables */
		for (vl_Sens = 1; vl_Sens < ERDTC_NB_CHAUSSEE; (vl_Sens == 4) ? vl_Sens += 3 : vl_Sens++)
			for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
			{
				/* Choix du pointeur en fonction de la frequence */
				if (va_Freq_Mes == ERDTC_MINUTE)
					pl_ERDT_Mes = &(pg_ERDT_Mes_1mn[vl_Num_RADT][vl_Sens][vl_Voie][va_Index]);
				else
					pl_ERDT_Mes = &(pg_ERDT_Mes_6mn[vl_Num_RADT][vl_Sens][vl_Voie][va_Index]);

				pl_ERDT_Mes->Horodate = va_Horodate;

				if (((va_TypeStation != ERDTC_ECHANGEUR) || (vl_Sens < 3)) &&
					((va_Freq_Mes == ERDTC_MINUTE) || (va_Type_Mes == ERDTC_NON_PL)))
				{
					pl_ERDT_Mes->Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
					pl_ERDT_Mes->Mesures.ValQ.Indic = XDC_RADT_INCONNU;
					pl_ERDT_Mes->Mesures.ValV.Val = XDC_RADT_V_INCONNU;
					pl_ERDT_Mes->Mesures.ValV.Indic = XDC_RADT_INCONNU;
					pl_ERDT_Mes->Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
					pl_ERDT_Mes->Mesures.ValTO.Indic = XDC_RADT_INCONNU;
				}
				if ((((va_TypeStation != ERDTC_ECHANGEUR) || (vl_Sens < 3)) && (va_Freq_Mes == ERDTC_MINUTE)) ||
					(va_Type_Mes == ERDTC_PL))
				{
					pl_ERDT_Mes->Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
					pl_ERDT_Mes->Mesures.ValPL.Indic = XDC_RADT_INCONNU;
				}
				if (((va_TypeStation != ERDTC_ECHANGEUR) || (vl_Sens < 3)) && (va_Freq_Mes == ERDTC_MINUTE))
				{
					pg_ERDT_Mes_6mn_gl[vl_Num_RADT][vl_Sens][vl_Voie][va_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
					pg_ERDT_Mes_6mn_gl[vl_Num_RADT][vl_Sens][vl_Voie][va_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
				}
			}

		/*B Si pas de station amont et pas de station aval ou Si la station a extrapole est de type echangeur alors pas d'extrapolation */
		if (((vl_Num_Station_Amont[0] == 0) && (vl_Num_Station_Aval[0] == 0) &&
			 (vl_Num_Station_Amont[1] == 0) && (vl_Num_Station_Aval[1] == 0)) ||
			(va_TypeStation == ERDTC_ECHANGEUR))
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "er_Extrapolation : Pas de station aval ni amont => Pas d'extrapolation ");
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : Sortie avec valeurs XDC_RADT_INCONNU");
			return (XDC_OK);
		}

		/*B Calcul des mesures de la station Amont si celle-ci existe */
		/*B Calcul de la somme de Q, V, TO et PL par voie de la chaussee */
		if ((vl_Num_Station_Amont[0] != 0) || (vl_Num_Station_Aval[0] != 0))
			vl_Sens = 1;
		if ((vl_Num_Station_Amont[1] != 0) || (vl_Num_Station_Aval[1] != 0))
			vl_Sens = 2;
		if (er_mesures_valides(va_Num_RADT, vl_Sens) >= 0)
			vl_Calcul_coef = 1;
		else
		{
			if (pm_coef_Extra_Q[vl_Num_RADT][0][0] != XDC_RADT_Q_INCONNU)
			{
				sprintf(pl_PathEtFic, "%s/Coef_Extra_Amont_R%d", ERDTC_PATH_FIC_DYN, va_Num_RADT);
				pl_Fd_Am = fopen(pl_PathEtFic, "r");
				sprintf(pl_PathEtFic, "%s/Coef_Extra_Aval_R%d", ERDTC_PATH_FIC_DYN, va_Num_RADT);
				pl_Fd_Av = fopen(pl_PathEtFic, "r");
				for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					if (pl_Fd_Am != NULL)
					{
						fgets(pl_PathEtFic, sizeof(pl_PathEtFic), pl_Fd_Am);
						if (!(feof(pl_Fd_Am)))
							sscanf(pl_PathEtFic, "Coef_Amont_Voie%d %lf %lf %lf %lf ", &vl_j,
								   &pm_coef_Extra_Q[vl_Num_RADT][1][vl_Voie], &pm_coef_Extra_V[vl_Num_RADT][1][vl_Voie],
								   &pm_coef_Extra_TO[vl_Num_RADT][1][vl_Voie], &pm_coef_Extra_PL[vl_Num_RADT][1][vl_Voie]);
					}
					if (pl_Fd_Av != NULL)
					{
						fgets(pl_PathEtFic, sizeof(pl_PathEtFic), pl_Fd_Av);
						if (!(feof(pl_Fd_Av)))
							sscanf(pl_PathEtFic, "Coef_Aval_Voie%d %lf %lf %lf %lf ", &vl_j,
								   &pm_coef_Extra_Q[vl_Num_RADT][2][vl_Voie], &pm_coef_Extra_V[vl_Num_RADT][2][vl_Voie],
								   &pm_coef_Extra_TO[vl_Num_RADT][2][vl_Voie], &pm_coef_Extra_PL[vl_Num_RADT][2][vl_Voie]);
					}
				}
				if (pl_Fd_Am != NULL)
					fclose(pl_Fd_Am);
				if (pl_Fd_Av != NULL)
					fclose(pl_Fd_Av);
			}
		}

		for (vl_Extra = 0; (vl_Calcul_coef && (vl_Extra < 3)); vl_Extra++)
		{
			for (vl_j = 0; vl_j < ERDTC_NB_VOIE_CHAUS; vl_j++)
			{
				pm_coef_Extra_V[vl_Num_RADT][vl_Extra][vl_j] = 0;
				pm_coef_Extra_Q[vl_Num_RADT][vl_Extra][vl_j] = 0;
				pm_coef_Extra_TO[vl_Num_RADT][vl_Extra][vl_j] = 0;
				pm_coef_Extra_PL[vl_Num_RADT][vl_Extra][vl_j] = 0;
			}

			switch (vl_Extra)
			{
			case 0:
				vl_Num_Station = ((vl_Num_Station_Amont[vl_Sens - 1] != 0) || (vl_Num_Station_Aval[vl_Sens - 1] != 0)) ? va_Num_RADT : 0;
				break;
			case 1:
				vl_Num_Station = vl_Num_Station_Amont[vl_Sens - 1];
				break;
			case 2:
				vl_Num_Station = vl_Num_Station_Aval[vl_Sens - 1];
				break;
			}
			if (vl_Num_Station != 0)
			{
				XZST_03EcritureTrace(XZSTC_DEBUG2, "%s :: XDC_RADT_CHAUSSEE  SENS %d :: Station <%d>", version, vl_Sens, vl_Num_Station);
				for (vl_j = 0; vl_j < ERDTC_NB_VOIE_CHAUS; vl_j++)
				{
					vl_Nb_Q[vl_j] = vl_Nb_V[vl_j] = vl_Nb_TO[vl_j] = vl_Nb_PL[vl_j] = 0;
				}

				for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
				{
					for (vl_i = 0; vl_i < ERDTC_NB_INDEX_6MN; vl_i++)
					{
						/* MAJ du pointeur  */
						pl_ERDT_Mes = &(pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_Station]][vl_Sens][vl_Voie][vl_i]);
						if ((pl_ERDT_Mes->Mesures.ValQ.Indic != XDC_RADT_INCONNU) || (pl_ERDT_Mes->Mesures.ValQ.Val != XDC_RADT_Q_INCONNU))
						{
							pm_coef_Extra_Q[vl_Num_RADT][vl_Extra][vl_Voie] += pl_ERDT_Mes->Mesures.ValQ.Val;
							vl_Nb_Q[vl_Voie]++;
						}

						if ((pl_ERDT_Mes->Mesures.ValV.Indic != XDC_RADT_INCONNU) || (pl_ERDT_Mes->Mesures.ValV.Val != XDC_RADT_V_INCONNU))
						{
							pm_coef_Extra_V[vl_Num_RADT][vl_Extra][vl_Voie] += pl_ERDT_Mes->Mesures.ValV.Val;
							vl_Nb_V[vl_Voie]++;
						}

						if ((pl_ERDT_Mes->Mesures.ValTO.Indic != XDC_RADT_INCONNU) || (pl_ERDT_Mes->Mesures.ValTO.Val != XDC_RADT_TO_INCONNU))
						{
							pm_coef_Extra_TO[vl_Num_RADT][vl_Extra][vl_Voie] += pl_ERDT_Mes->Mesures.ValTO.Val;
							vl_Nb_TO[vl_Voie]++;
						}

						if ((pl_ERDT_Mes->Mesures.ValPL.Indic != XDC_RADT_INCONNU) || (pl_ERDT_Mes->Mesures.ValPL.Val != XDC_RADT_PL_INCONNU))
						{
							pm_coef_Extra_PL[vl_Num_RADT][vl_Extra][vl_Voie] += pl_ERDT_Mes->Mesures.ValPL.Val;
							vl_Nb_PL[vl_Voie]++;
							XZST_03EcritureTrace(XZSTC_DEBUG1, "%s :: ==> PL = %lf :: NbPL = %d", version, pm_coef_Extra_PL[vl_Num_RADT][vl_Extra][vl_Voie], vl_Nb_PL[vl_Voie]);
						}
					} /* fin for  vl_i */

					if ((vl_Nb_Q[vl_Voie] > 0) && (pm_coef_Extra_Q[vl_Num_RADT][vl_Extra][vl_Voie] >= 0.0))
						pm_coef_Extra_Q[vl_Num_RADT][vl_Extra][vl_Voie] = ((double)pm_coef_Extra_Q[vl_Num_RADT][vl_Extra][vl_Voie] / vl_Nb_Q[vl_Voie]);
					else
						pm_coef_Extra_Q[vl_Num_RADT][vl_Extra][vl_Voie] = 0;

					if ((vl_Nb_V[vl_Voie] > 0) && (pm_coef_Extra_V[vl_Num_RADT][vl_Extra][vl_Voie] >= 0.0) && (va_TypeStation != ERDTC_PEAGE))
						pm_coef_Extra_V[vl_Num_RADT][vl_Extra][vl_Voie] = ((double)pm_coef_Extra_V[vl_Num_RADT][vl_Extra][vl_Voie] / vl_Nb_V[vl_Voie]);
					else
						pm_coef_Extra_V[vl_Num_RADT][vl_Extra][vl_Voie] = 0;

					if ((vl_Nb_TO[vl_Voie] > 0) && (pm_coef_Extra_TO[vl_Num_RADT][vl_Extra][vl_Voie] >= 0.0) && (va_TypeStation != ERDTC_PEAGE))
						pm_coef_Extra_TO[vl_Num_RADT][vl_Extra][vl_Voie] = ((double)pm_coef_Extra_TO[vl_Num_RADT][vl_Extra][vl_Voie] / vl_Nb_TO[vl_Voie]);
					else
						pm_coef_Extra_TO[vl_Num_RADT][vl_Extra][vl_Voie] = 0;

					if ((vl_Nb_PL[vl_Voie] > 0) && (pm_coef_Extra_PL[vl_Num_RADT][vl_Extra][vl_Voie] >= 0.0) && (va_TypeStation != ERDTC_PEAGE))
						pm_coef_Extra_PL[vl_Num_RADT][vl_Extra][vl_Voie] = ((double)pm_coef_Extra_PL[vl_Num_RADT][vl_Extra][vl_Voie] / vl_Nb_PL[vl_Voie]);
					else
						pm_coef_Extra_PL[vl_Num_RADT][vl_Extra][vl_Voie] = 0;

					XZST_03EcritureTrace(XZSTC_DEBUG2, "%s :MOY: St%d S%d V%d : Q = %lf :: V = %lf :: TO = %lf :: PL = %lf", version, vl_Num_Station, vl_Sens, vl_Voie,
										 pm_coef_Extra_Q[vl_Num_RADT][vl_Extra][vl_Voie], pm_coef_Extra_V[vl_Num_RADT][vl_Extra][vl_Voie], pm_coef_Extra_TO[vl_Num_RADT][vl_Extra][vl_Voie], pm_coef_Extra_PL[vl_Num_RADT][vl_Extra][vl_Voie]);
					XZST_03EcritureTrace(XZSTC_DEBUG2, "%s :MOY: St%d S%d V%d : NbQ = %d :: NbV = %d :: NbTO = %d :: NbPL = %d", version, vl_Num_Station, vl_Sens, vl_Voie,
										 vl_Nb_Q[vl_Voie], vl_Nb_V[vl_Voie], vl_Nb_TO[vl_Voie], vl_Nb_PL[vl_Voie]);
				} /* fin  for vl_Voie */
			}	  /* fin if vl_Num_Station != 0  */
		}		  /* fin */

		if (vl_Calcul_coef)
		{
			if ((vl_Num_Station_Amont[0] != 0) || (vl_Num_Station_Amont[1] != 0))
			{
				sprintf(pl_PathEtFic, "%s/Coef_Extra_Amont_R%d", ERDTC_PATH_FIC_DYN, va_Num_RADT);
				pl_Fd_Am = fopen(pl_PathEtFic, "w");
			}
			if ((vl_Num_Station_Aval[0] != 0) || (vl_Num_Station_Aval[1] != 0))
			{
				sprintf(pl_PathEtFic, "%s/Coef_Extra_Aval_R%d", ERDTC_PATH_FIC_DYN, va_Num_RADT);
				pl_Fd_Av = fopen(pl_PathEtFic, "w");
			}
			for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
			{
				if ((vl_Num_Station_Amont[0] != 0) || (vl_Num_Station_Amont[1] != 0))
				{
					if (pm_coef_Extra_Q[vl_Num_RADT][1][vl_Voie] != 0)
						pm_coef_Extra_Q[vl_Num_RADT][1][vl_Voie] = pm_coef_Extra_Q[vl_Num_RADT][0][vl_Voie] / pm_coef_Extra_Q[vl_Num_RADT][1][vl_Voie];
					if (pm_coef_Extra_V[vl_Num_RADT][1][vl_Voie] != 0)
						pm_coef_Extra_V[vl_Num_RADT][1][vl_Voie] = pm_coef_Extra_V[vl_Num_RADT][0][vl_Voie] / pm_coef_Extra_V[vl_Num_RADT][1][vl_Voie];
					if (pm_coef_Extra_TO[vl_Num_RADT][1][vl_Voie] != 0)
						pm_coef_Extra_TO[vl_Num_RADT][1][vl_Voie] = pm_coef_Extra_TO[vl_Num_RADT][0][vl_Voie] / pm_coef_Extra_TO[vl_Num_RADT][1][vl_Voie];
					if (pm_coef_Extra_PL[vl_Num_RADT][1][vl_Voie] != 0)
						pm_coef_Extra_PL[vl_Num_RADT][1][vl_Voie] = pm_coef_Extra_PL[vl_Num_RADT][0][vl_Voie] / pm_coef_Extra_PL[vl_Num_RADT][1][vl_Voie];
					if (pl_Fd_Am != NULL)
						fprintf(pl_Fd_Am, "Coef_Amont_Voie%d %lf %lf %lf %lf \n", vl_Voie,
								pm_coef_Extra_Q[vl_Num_RADT][1][vl_Voie], pm_coef_Extra_V[vl_Num_RADT][1][vl_Voie],
								pm_coef_Extra_TO[vl_Num_RADT][1][vl_Voie], pm_coef_Extra_PL[vl_Num_RADT][1][vl_Voie]);
				}
				if ((vl_Num_Station_Aval[0] != 0) || (vl_Num_Station_Aval[1] != 0))
				{
					if (pm_coef_Extra_Q[vl_Num_RADT][2][vl_Voie] != 0)
						pm_coef_Extra_Q[vl_Num_RADT][2][vl_Voie] = pm_coef_Extra_Q[vl_Num_RADT][0][vl_Voie] / pm_coef_Extra_Q[vl_Num_RADT][2][vl_Voie];
					if (pm_coef_Extra_V[vl_Num_RADT][2][vl_Voie] != 0)
						pm_coef_Extra_V[vl_Num_RADT][2][vl_Voie] = pm_coef_Extra_V[vl_Num_RADT][0][vl_Voie] / pm_coef_Extra_V[vl_Num_RADT][2][vl_Voie];
					if (pm_coef_Extra_TO[vl_Num_RADT][2][vl_Voie] != 0)
						pm_coef_Extra_TO[vl_Num_RADT][2][vl_Voie] = pm_coef_Extra_TO[vl_Num_RADT][0][vl_Voie] / pm_coef_Extra_TO[vl_Num_RADT][2][vl_Voie];
					if (pm_coef_Extra_PL[vl_Num_RADT][2][vl_Voie] != 0)
						pm_coef_Extra_PL[vl_Num_RADT][2][vl_Voie] = pm_coef_Extra_PL[vl_Num_RADT][0][vl_Voie] / pm_coef_Extra_PL[vl_Num_RADT][2][vl_Voie];
					if (pl_Fd_Av != NULL)
						fprintf(pl_Fd_Av, "Coef_Aval_Voie%d %lf %lf %lf %lf \n", vl_Voie,
								pm_coef_Extra_Q[vl_Num_RADT][2][vl_Voie], pm_coef_Extra_V[vl_Num_RADT][2][vl_Voie],
								pm_coef_Extra_TO[vl_Num_RADT][2][vl_Voie], pm_coef_Extra_PL[vl_Num_RADT][2][vl_Voie]);
				}
			}
			if (pl_Fd_Am != NULL)
				fclose(pl_Fd_Am);
			if (pl_Fd_Av != NULL)
				fclose(pl_Fd_Av);
			pm_coef_Extra_Q[vl_Num_RADT][0][0] = XDC_RADT_Q_INCONNU;
		}

		for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
		{
			/* MAJ du pointeur  */
			if (vl_Num_Station_Amont[vl_Sens - 1] != 0)
				pl_Mes_Am = &(pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_Station_Amont[vl_Sens - 1]]][vl_Sens][vl_Voie][vl_Index_Station_Amont[vl_Sens - 1]]);
			else
				pl_Mes_Am = NULL;
			if (vl_Num_Station_Aval[vl_Sens - 1] != 0)
				pl_Mes_Av = &(pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_Station_Aval[vl_Sens - 1]]][vl_Sens][vl_Voie][vl_Index_Station_Aval[vl_Sens - 1]]);
			else
				pl_Mes_Av = NULL;

			vl_div = vl_Am = vl_Av = 0;
			if ((pm_coef_Extra_Q[vl_Num_RADT][1][vl_Voie] != 0) && (pl_Mes_Am != NULL) && (pl_Mes_Am->Mesures.ValQ.Val != XDC_RADT_Q_INCONNU))
			{
				vl_div = 1;
				vl_Am = pl_Mes_Am->Mesures.ValQ.Val;
			}
			if ((pm_coef_Extra_Q[vl_Num_RADT][2][vl_Voie] != 0) && (pl_Mes_Av != NULL) && (pl_Mes_Av->Mesures.ValQ.Val != XDC_RADT_Q_INCONNU))
			{
				vl_div++;
				vl_Av = pl_Mes_Av->Mesures.ValQ.Val;
			}
			if (((vl_Am != 0) || (vl_Av != 0)) && (vl_div != 0))
				vl_Q_Moy[vl_Voie] = (pm_coef_Extra_Q[vl_Num_RADT][1][vl_Voie] * vl_Am + pm_coef_Extra_Q[vl_Num_RADT][2][vl_Voie] * vl_Av) / vl_div;
			else
				vl_Q_Moy[vl_Voie] = XDC_RADT_Q_INCONNU;

			vl_div = vl_Am = vl_Av = 0;
			if ((pm_coef_Extra_V[vl_Num_RADT][1][vl_Voie] != 0) && (pl_Mes_Am != NULL) && (pl_Mes_Am->Mesures.ValV.Val != XDC_RADT_V_INCONNU))
			{
				vl_div = 1;
				vl_Am = pl_Mes_Am->Mesures.ValV.Val;
			}
			if ((pm_coef_Extra_V[vl_Num_RADT][2][vl_Voie] != 0) && (pl_Mes_Av != NULL) && (pl_Mes_Av->Mesures.ValV.Val != XDC_RADT_V_INCONNU))
			{
				vl_div++;
				vl_Av = pl_Mes_Av->Mesures.ValV.Val;
			}
			if (((vl_Am != 0) || (vl_Av != 0)) && (vl_div != 0))
				vl_V_Moy[vl_Voie] = (pm_coef_Extra_V[vl_Num_RADT][1][vl_Voie] * vl_Am + pm_coef_Extra_V[vl_Num_RADT][2][vl_Voie] * vl_Av) / vl_div;
			else
				vl_V_Moy[vl_Voie] = XDC_RADT_V_INCONNU;

			vl_div = vl_Am = vl_Av = 0;
			if ((pm_coef_Extra_TO[vl_Num_RADT][1][vl_Voie] != 0) && (pl_Mes_Am != NULL) && (pl_Mes_Am->Mesures.ValTO.Val != XDC_RADT_TO_INCONNU))
			{
				vl_div = 1;
				vl_Am = pl_Mes_Am->Mesures.ValTO.Val;
			}
			if ((pm_coef_Extra_TO[vl_Num_RADT][2][vl_Voie] != 0) && (pl_Mes_Av != NULL) && (pl_Mes_Av->Mesures.ValTO.Val != XDC_RADT_TO_INCONNU))
			{
				vl_div++;
				vl_Av = pl_Mes_Av->Mesures.ValTO.Val;
			}
			if (((vl_Am != 0) || (vl_Av != 0)) && (vl_div != 0))
				vl_TO_Moy[vl_Voie] = (pm_coef_Extra_TO[vl_Num_RADT][1][vl_Voie] * vl_Am + pm_coef_Extra_TO[vl_Num_RADT][2][vl_Voie] * vl_Av) / vl_div;
			else
				vl_TO_Moy[vl_Voie] = XDC_RADT_TO_INCONNU;

			vl_div = vl_Am = vl_Av = 0;
			if ((pm_coef_Extra_PL[vl_Num_RADT][1][vl_Voie] != 0) && (pl_Mes_Am != NULL) && (pl_Mes_Am->Mesures.ValPL.Val != XDC_RADT_PL_INCONNU))
			{
				vl_div = 1;
				vl_Am = pl_Mes_Am->Mesures.ValPL.Val;
			}
			if ((pm_coef_Extra_PL[vl_Num_RADT][2][vl_Voie] != 0) && (pl_Mes_Av != NULL) && (pl_Mes_Av->Mesures.ValPL.Val != XDC_RADT_PL_INCONNU))
			{
				vl_div++;
				vl_Am = pl_Mes_Av->Mesures.ValPL.Val;
			}
			if (((vl_Am != 0) || (vl_Av != 0)) && (vl_div != 0))
				vl_PL_Moy[vl_Voie] = (pm_coef_Extra_PL[vl_Num_RADT][1][vl_Voie] * vl_Am + pm_coef_Extra_PL[vl_Num_RADT][2][vl_Voie] * vl_Av) / vl_div;
			else
				vl_PL_Moy[vl_Voie] = XDC_RADT_PL_INCONNU;

			if ((vl_Num_Station_Amont[0] != 0) || (vl_Num_Station_Amont[1] != 0))
			{
				XZST_03EcritureTrace(XZSTC_DEBUG2, "%s :CoefAM: St%d S%d V%d : Q = %lf :: V = %lf :: TO = %lf :: PL = %lf", version, vl_Num_Station_Amont[vl_Sens - 1], vl_Sens, vl_Voie,
									 pm_coef_Extra_Q[vl_Num_RADT][1][vl_Voie], pm_coef_Extra_V[vl_Num_RADT][1][vl_Voie],
									 pm_coef_Extra_TO[vl_Num_RADT][1][vl_Voie], pm_coef_Extra_PL[vl_Num_RADT][1][vl_Voie]);
				XZST_03EcritureTrace(XZSTC_DEBUG2, "%s :Val AM: S%dV%d: Q = %d I=%d :: V = %d I=%d :: TO = %d I=%d :: PL = %d I=%d", version, vl_Sens, vl_Voie,
									 pl_Mes_Am->Mesures.ValQ.Val,
									 pl_Mes_Am->Mesures.ValQ.Indic,
									 pl_Mes_Am->Mesures.ValV.Val,
									 pl_Mes_Am->Mesures.ValV.Indic,
									 pl_Mes_Am->Mesures.ValTO.Val,
									 pl_Mes_Am->Mesures.ValTO.Indic,
									 pl_Mes_Am->Mesures.ValPL.Val,
									 pl_Mes_Am->Mesures.ValPL.Indic);
			}
			if ((vl_Num_Station_Aval[0] != 0) || (vl_Num_Station_Aval[1] != 0))
			{
				XZST_03EcritureTrace(XZSTC_DEBUG2, "%s :CoefAV: St%d S%d V%d : Q = %lf :: V = %lf :: TO = %lf :: PL = %lf", version, vl_Num_Station_Aval[vl_Sens - 1], vl_Sens, vl_Voie,
									 pm_coef_Extra_Q[vl_Num_RADT][2][vl_Voie], pm_coef_Extra_V[vl_Num_RADT][2][vl_Voie],
									 pm_coef_Extra_TO[vl_Num_RADT][2][vl_Voie], pm_coef_Extra_PL[vl_Num_RADT][2][vl_Voie]);
				XZST_03EcritureTrace(XZSTC_DEBUG2, "%s :Val AV: S%dV%d: Q = %d I=%d :: V = %d I=%d :: TO = %d I=%d :: PL = %d I=%d", version, vl_Sens, vl_Voie,
									 pl_Mes_Av->Mesures.ValQ.Val,
									 pl_Mes_Av->Mesures.ValQ.Indic,
									 pl_Mes_Av->Mesures.ValV.Val,
									 pl_Mes_Av->Mesures.ValV.Indic,
									 pl_Mes_Av->Mesures.ValTO.Val,
									 pl_Mes_Av->Mesures.ValTO.Indic,
									 pl_Mes_Av->Mesures.ValPL.Val,
									 pl_Mes_Av->Mesures.ValPL.Indic);
			}
			XZST_03EcritureTrace(XZSTC_DEBUG2, "%s :Val   : St%d S%d V%d : Q = %lf :: V = %lf :: TO = %lf :: PL = %lf", version, va_Num_RADT, vl_Sens, vl_Voie,
								 vl_Q_Moy[vl_Voie], vl_V_Moy[vl_Voie], vl_TO_Moy[vl_Voie], vl_PL_Moy[vl_Voie]);
		}

		/*B On deduit les valeurs a extrapolees pour la voie passee en parametre */
		for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
		{
			/* Choix du pointeur en fonction de la frequence */
			pl_ERDT_Mes = (va_Freq_Mes == ERDTC_MINUTE) ? &(pg_ERDT_Mes_1mn[vl_Num_RADT][vl_Sens][vl_Voie][va_Index]) : &(pg_ERDT_Mes_6mn[vl_Num_RADT][vl_Sens][vl_Voie][va_Index]);
			if ((va_Type_Mes == ERDTC_NON_PL) || (va_Freq_Mes == ERDTC_MINUTE))
			{
				pl_ERDT_Mes->Mesures.ValQ.Val = (short)vl_Q_Moy[vl_Voie];
				pl_ERDT_Mes->Mesures.ValQ.Indic = (vl_Q_Moy[vl_Voie] != XDC_RADT_Q_INCONNU) ? XDC_RADT_RECONST : XDC_RADT_INCONNU;
				pl_ERDT_Mes->Mesures.ValV.Val = (short)vl_V_Moy[vl_Voie];
				pl_ERDT_Mes->Mesures.ValV.Indic = (vl_V_Moy[vl_Voie] != XDC_RADT_V_INCONNU) ? XDC_RADT_RECONST : XDC_RADT_INCONNU;
				pl_ERDT_Mes->Mesures.ValTO.Val = (short)vl_TO_Moy[vl_Voie];
				pl_ERDT_Mes->Mesures.ValTO.Indic = (vl_TO_Moy[vl_Voie] != XDC_RADT_TO_INCONNU) ? XDC_RADT_RECONST : XDC_RADT_INCONNU;
			}

			if ((va_Type_Mes == ERDTC_PL) || (va_Freq_Mes == ERDTC_MINUTE))
			{
				pl_ERDT_Mes->Mesures.ValPL.Val = (short)vl_PL_Moy[vl_Voie];
				pl_ERDT_Mes->Mesures.ValPL.Indic = (vl_PL_Moy[vl_Voie] != XDC_RADT_PL_INCONNU) ? XDC_RADT_RECONST : XDC_RADT_INCONNU;
			}

			if (va_Freq_Mes == ERDTC_MINUTE)
			{
				pg_ERDT_Mes_6mn_gl[vl_Num_RADT][vl_Sens][vl_Voie][va_Index].Mesures.ValPL.Val = pl_ERDT_Mes->Mesures.ValPL.Val;
				pg_ERDT_Mes_6mn_gl[vl_Num_RADT][vl_Sens][vl_Voie][va_Index].Mesures.ValPL.Indic = pl_ERDT_Mes->Mesures.ValPL.Indic;
			}

			XZST_03EcritureTrace(XZSTC_DEBUG3, " %s ::S%dV%d: Q = %d I=%d :: V = %d I=%d :: TO = %d I=%d :: PL = %d I=%d", version, vl_Sens, vl_Voie,
								 pl_ERDT_Mes->Mesures.ValQ.Val,
								 pl_ERDT_Mes->Mesures.ValQ.Indic,
								 pl_ERDT_Mes->Mesures.ValV.Val,
								 pl_ERDT_Mes->Mesures.ValV.Indic,
								 pl_ERDT_Mes->Mesures.ValTO.Val,
								 pl_ERDT_Mes->Mesures.ValTO.Indic,
								 pl_ERDT_Mes->Mesures.ValPL.Val,
								 pl_ERDT_Mes->Mesures.ValPL.Indic);
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s", version);
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *
 *  Calcul des donnees par chaussee en fonction des donnees par voies
 *  pour une station RADT
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int er_Calcul_Chaussee(XDY_Eqt va_Num_RADT,
					   XDY_Mot va_TypeStation,
					   XDY_Entier va_Index,
					   XDY_Horodate va_Horodate)

/*
 * ARGUMENTS EN ENTREE :
 *
 *   va_Num_RADT : Numero de la station RADT
 *   va_Index : Numero de l'index du tableau de valeurs
 *
 * ARGUMENTS EN SORTIE :
 *
 *   pa_Mesure   : Mesure Chaussee
 *
 * CODE RETOUR :
 *   XDC_OK	succes
 *   XDC_NOK	echec
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *
 * Calcul des donnees par chaussee en fonction des donnees par voies pour une station RADT
 *
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ : er_Calcul_Chaussee";

	XDY_Horodate vl_Horodate = 0.0;
	XZSMT_Horodate pl_Horodatedecompo;
	char vl_heure_tab[3];
	char vl_minute_tab[3];
	int vl_Horo_en_mn = 0;
	int vl_minute = 0;
	int vl_heure = 0;
	XDY_Eqt vl_Num_RADT = 0;
	XDY_Sens vl_Sens = 0;
	XDY_Voie vl_Chaussee = 0;
	XDY_Entier vl_Index = 0;
	XDY_Entier vl_i = 0;
	XDY_Entier vl_Voie = 0;
	XDY_Coeff ValQ[ERDTC_NB_CHAUSSEE];
	XDY_Coeff ValV[ERDTC_NB_CHAUSSEE];
	XDY_Coeff ValTO[ERDTC_NB_CHAUSSEE];
	XDY_Coeff ValPL[ERDTC_NB_CHAUSSEE];
	XDY_Entier vl_Nb_Voie_Ch[ERDTC_NB_CHAUSSEE];
	float val1, val2, val3;
	int valint;
	int j = 0;
	int k = 0;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : er_Calcul_Chaussee Index = %d\tRADT = %d", va_Index, va_Num_RADT);

	for (vl_i = 0; vl_i < ERDTC_NB_CHAUSSEE; vl_i++)
	{
		vl_Nb_Voie_Ch[vl_i] = 0;
		ValQ[vl_i] = -1;
		ValV[vl_i] = -1;
		ValTO[vl_i] = -1;
		ValPL[vl_i] = -1;
	}

	/*DEM476: si (( 07.30 < Heure < 22.00) et (Q == 0)) alors ne pas creer d'enregistrement en base pour la  voie 0 */
	/* Convertir l'horodate recu en sec en JJ/MM/AAAA HH:MM:SS */
	XZSM_11ConversionHorodate(va_Horodate, &pl_Horodatedecompo);
	strncpy(vl_heure_tab, &pl_Horodatedecompo[11], 2);
	vl_heure_tab[2] = '\0';
	vl_heure = atoi(vl_heure_tab);
	strncpy(vl_minute_tab, &pl_Horodatedecompo[14], 2);
	vl_minute_tab[2] = '\0';
	vl_minute = atoi(vl_minute_tab);

	/*07.30 = 450 minutes et 22.00 = 1320 minutes */
	vl_Horo_en_mn = ((vl_heure * 60) + vl_minute);
	j = 0;

	/*A Calcul des donnees a partir des donnees */
	for (vl_Chaussee = 1; vl_Chaussee < ERDTC_NB_CHAUSSEE; (vl_Chaussee == 4) ? vl_Chaussee += 3 : vl_Chaussee++)
	{

		switch (vl_Chaussee)
		{
			/*B Cas de la chaussee Sens 1 */
		case 1:
			for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
			{
				/*if (j != 1)*/
				/*{*/
				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_NORMAL)
				{
					k = 1;
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic != XDC_RADT_INCONNU)
					{
						ValQ[vl_Chaussee] = (((ValQ[vl_Chaussee] == -1) ? 0 : ValQ[vl_Chaussee]) +
											 (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val));
						XZST_03EcritureTrace(XZSTC_INFO, "Calcul du debit chaussee, sens 1, voie=%d, ValQ[vl_Chaussee]=%f",
											 vl_Voie, ValQ[vl_Chaussee]);
					}
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val > 0.0) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic != XDC_RADT_INCONNU))
						ValV[vl_Chaussee] = (((ValV[vl_Chaussee] == -1) ? 0 : ValV[vl_Chaussee]) +
											 ((float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val / pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val));

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic != XDC_RADT_INCONNU)
						ValTO[vl_Chaussee] = ((ValTO[vl_Chaussee] == -1) ? 0 : ValTO[vl_Chaussee]) +
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val;

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic != XDC_RADT_INCONNU)
						ValPL[vl_Chaussee] = ((ValPL[vl_Chaussee] == -1) ? 0 : (float)ValPL[vl_Chaussee]) +
											 (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val *
											  pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);

					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val == 0) && (vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) && (vl_Horo_en_mn < pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "DEM476 voie=%d, sens=%d :sens normal avec debit nul avec heure ds tranche", vl_Voie, vl_Chaussee);
						ValQ[vl_Chaussee] = -1;
						ValV[vl_Chaussee] = -1;
						ValTO[vl_Chaussee] = -1;
						ValPL[vl_Chaussee] = -1;
						break;
					}

					vl_Nb_Voie_Ch[vl_Chaussee]++;
				} /* fin condition sur XDC_VOIE_SENS_NORMAL*/

				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_INVERSE)
				{
					k = 2;
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic != XDC_RADT_INCONNU)
					{
						ValQ[vl_Chaussee + 1] = ((ValQ[vl_Chaussee + 1] == -1) ? 0 : ValQ[vl_Chaussee + 1]) +
												(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);
						XZST_03EcritureTrace(XZSTC_INFO, "Calcul du debit chaussee, voie inverse, sens 1, voie=%d, ValQ[vl_Chaussee]=%f", vl_Voie,
											 ValQ[vl_Chaussee + 1]);
					}
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val > 0.0) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic != XDC_RADT_INCONNU))
						ValV[vl_Chaussee + 1] = ((ValV[vl_Chaussee + 1] == -1) ? 0 : ValV[vl_Chaussee + 1]) +
												((float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val / pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val);

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic != XDC_RADT_INCONNU)
						ValTO[vl_Chaussee + 1] = ((ValTO[vl_Chaussee + 1] == -1) ? 0 : ValTO[vl_Chaussee + 1]) +
												 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val;

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic != XDC_RADT_INCONNU)
						ValPL[vl_Chaussee + 1] = ((ValPL[vl_Chaussee + 1] == -1) ? 0 : (float)ValPL[vl_Chaussee + 1]) +
												 ((float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val *
												  (float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);

					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val == 0) && (vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) && (vl_Horo_en_mn < pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "DEM476 voie=%d, sens=%d :sens inverse avec debit nul avec heure ds tranche", vl_Voie, vl_Chaussee);
						ValQ[vl_Chaussee + 1] = -1;
						ValV[vl_Chaussee + 1] = -1;
						ValTO[vl_Chaussee + 1] = -1;
						ValPL[vl_Chaussee + 1] = -1;
						break;
					}
					vl_Nb_Voie_Ch[vl_Chaussee + 1]++;
				} /* fin condition sur XDC_VOIE_SENS_INVERSE */
				  /* }fin condition sur j != 1 */
			}	  /*fin boucle for */
			break;

			/*B Cas de la chaussee Sens 2 */
		case 2:
			for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
			{
				/*{*/
				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_NORMAL)
				{
					k = 3;
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic != XDC_RADT_INCONNU)
					{
						ValQ[vl_Chaussee] = ((ValQ[vl_Chaussee] == -1) ? 0 : ValQ[vl_Chaussee]) +
											(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);
						XZST_03EcritureTrace(XZSTC_INFO, "Calcul du debit chaussee %d, sens 2, voie=%d, ValQ[vl_Chaussee]=%f",
											 vl_Chaussee, vl_Voie, ValQ[vl_Chaussee]);
					}
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val > 0.0) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic != XDC_RADT_INCONNU))
						ValV[vl_Chaussee] = ((ValV[vl_Chaussee] == -1) ? 0 : ValV[vl_Chaussee]) +
											((float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val / pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val);

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic != XDC_RADT_INCONNU)
						ValTO[vl_Chaussee] = ((ValTO[vl_Chaussee] == -1) ? 0 : ValTO[vl_Chaussee]) +
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val;

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic != XDC_RADT_INCONNU)
						ValPL[vl_Chaussee] = ((ValPL[vl_Chaussee] == -1) ? 0 : (float)ValPL[vl_Chaussee]) +
											 (float)(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val *
													 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);

					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val == 0) &&
						(vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) &&
						(vl_Horo_en_mn < pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "DEM476 station=%d, voie=%d, sens=%d :sens normal 2  avec debit nul avec heure ds tranche", va_Num_RADT, vl_Voie, vl_Chaussee);
						ValQ[vl_Chaussee] = -1;
						ValV[vl_Chaussee] = -1;
						ValTO[vl_Chaussee] = -1;
						ValPL[vl_Chaussee] = -1;
						break;
					}

					vl_Nb_Voie_Ch[vl_Chaussee]++;
				} /* fin condition sur XDC_VOIE_SENS_NORMAL */

				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie == XDC_VOIE_SENS_INVERSE)
				{
					k = 4;
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic != XDC_RADT_INCONNU)
					{
						ValQ[vl_Chaussee - 1] = ((ValQ[vl_Chaussee - 1] == -1) ? 0 : ValQ[vl_Chaussee - 1]) +
												(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);
						XZST_03EcritureTrace(XZSTC_INFO, "Calcul du debit chaussee, voie inverse, sens 2, voie=%d, ValQ[[vl_Chaussee]=%f", vl_Voie,
											 ValQ[vl_Chaussee - 1]);
					}
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val > 0.0) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic != XDC_RADT_INCONNU))
						ValV[vl_Chaussee - 1] = ((ValV[vl_Chaussee - 1] == -1) ? 0 : ValV[vl_Chaussee - 1]) +
												((float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val / pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val);

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic != XDC_RADT_INCONNU)
						ValTO[vl_Chaussee - 1] = ((ValTO[vl_Chaussee - 1] == -1) ? 0 : ValTO[vl_Chaussee - 1]) +
												 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val;

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic != XDC_RADT_INCONNU)
						ValPL[vl_Chaussee - 1] = ((ValPL[vl_Chaussee - 1] == -1) ? 0 : (float)ValPL[vl_Chaussee - 1]) +
												 ((float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val *
												  (float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val == 0) &&
						(vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) &&
						(vl_Horo_en_mn < pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "DEM476 station=%d, voie=%d, sens=%d :sens inverse avec debit nul avec heure ds tranche", va_Num_RADT, vl_Voie, vl_Chaussee);
						ValQ[vl_Chaussee - 1] = -1;
						ValV[vl_Chaussee - 1] = -1;
						ValTO[vl_Chaussee - 1] = -1;
						ValPL[vl_Chaussee - 1] = -1;
						break;
					}
					vl_Nb_Voie_Ch[vl_Chaussee - 1]++;
				} /* fin condition sur XDC_VOIE_SENS_INVERSE */

				/* } fin condition sur j != 1 */
			} /*fin boucle for */
			break;

			/*B Cas par defaut */
		default:
			for (vl_Voie = 1; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
			{
				/*if ( j != 1)*/
				/*{*/
				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie != XDC_VOIE_INCONNUE)
				{
					k = 5;
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic != XDC_RADT_INCONNU)
					{
						ValQ[vl_Chaussee] = ((ValQ[vl_Chaussee] == -1) ? 0 : ValQ[vl_Chaussee]) +
											(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);
						XZST_03EcritureTrace(XZSTC_INFO, "Calcul du debit chaussee, cas par defaut, chaussee %d, voie= %d, ValQ[vl_Chaussee]=%f",
											 vl_Chaussee, vl_Voie, ValQ[vl_Chaussee]);
					}

					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val > 0.0) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic != XDC_RADT_INCONNU))
						ValV[vl_Chaussee] = ((ValV[vl_Chaussee] == -1) ? 0 : ValV[vl_Chaussee]) +
											((float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val / pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val);

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic != XDC_RADT_INCONNU)
						ValTO[vl_Chaussee] = ((ValTO[vl_Chaussee] == -1) ? 0 : ValTO[vl_Chaussee]) +
											 (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val);

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic != XDC_RADT_INCONNU)
						ValPL[vl_Chaussee] = ((ValPL[vl_Chaussee] == -1) ? 0 : (float)ValPL[vl_Chaussee]) +
											 ((float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val *
											  (float)pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);

					/*
							   if (( pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val == 0) && (vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) && (vl_Horo_en_mn<pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur))
								{
								XZST_03EcritureTrace( XZSTC_INFO,"DEM476 station=%d, voie=%d, sens=%d :debit nul avec heure ds tranche",
																	va_Num_RADT,vl_Voie,vl_Chaussee);
								ValQ[vl_Chaussee] = -1;
								ValV[vl_Chaussee] = -1;
								ValTO[vl_Chaussee] = -1;
								ValPL[vl_Chaussee] = -1;
								break;
								} */
					vl_Nb_Voie_Ch[vl_Chaussee]++;
				} /* fin condition sur XDC_VOIE_INCONNUE */
				  /* } fin condition sur j != 1 */
			}
			break;

		} /* fin switch */
	}	  /* fin for vl_Chaussee */

	/*B Mise a jour du tableau des 6mn QVTO */
	for (vl_Chaussee = 1; vl_Chaussee < ERDTC_NB_CHAUSSEE; (vl_Chaussee == 4) ? vl_Chaussee += 3 : vl_Chaussee++)
	{

		if (vl_Nb_Voie_Ch[vl_Chaussee] > 0)
		{
			/*DEM476: si ((sens=1 ou sens=2) et voie=0 et Q<50) alors pas d'enregistrement en base de donnees */

			if (((vl_Chaussee == 1) || (vl_Chaussee == 2)) && (ValQ[vl_Chaussee] < pg_Tab_REG_PARAMS[XDC_REG_DEB_CHAUSSEE_MIN].Valeur) && ((vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) && (vl_Horo_en_mn < pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur)) && (ValQ[vl_Chaussee] != -1))
			{
				XZST_03EcritureTrace(XZSTC_FONCTION, "DEM476 cas if ( ((vl_Chaussee == 1) || (vl_Chaussee == 2)) && (ValQ[vl_Chaussee] < XDC_REG_DEB_CHAUSSEE_MIN");
				XZST_03EcritureTrace(XZSTC_INFO, "caro chaussee=%d, k=%d, ValQ[vl_Chaussee]=%f", vl_Chaussee, k, ValQ[vl_Chaussee]);
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate = va_Horodate;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
			} /* fin condition sur ValQ[vl_Chaussee] < 50 */
			else
			{
				/* Regle pas de donn�e le jour = donn�e la nuit ignor�e, setup d'une variable */
				if ((vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) &&
					(vl_Horo_en_mn < pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur))
				{
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val != -1) ||
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val != -1))
					{
						pg_Nb_Donnees_Jour[va_Num_RADT][vl_Chaussee] = 1;
					}
				}

				/* FIN Regle pas de donn�e le jour = donn�e la nuit ignor�e */

				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate = va_Horodate;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = (ValQ[vl_Chaussee] == -1) ? XDC_RADT_Q_INCONNU : ValQ[vl_Chaussee];
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = (ValQ[vl_Chaussee] == -1) ? XDC_RADT_INCONNU : XDC_RADT_CORRECT;
				if (ValV[vl_Chaussee] > 0.0)
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = ((float)ValQ[vl_Chaussee] / ValV[vl_Chaussee]);
				/*					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = ((vl_Chaussee > 2) || (va_TypeStation == ERDTC_PEAGE)) ? XDC_RADT_V_INCONNU : ( (float) ValQ[vl_Chaussee] / ValV[vl_Chaussee] ) ;*/
				else if (ValV[vl_Chaussee] == 0.0)
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = ((vl_Chaussee > 2) || (va_TypeStation == ERDTC_PEAGE)) ? XDC_RADT_V_INCONNU : 0;
				else
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_CORRECT;
				/*				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = ((vl_Chaussee > 2) || (va_TypeStation == ERDTC_PEAGE) || (ValV[vl_Chaussee] == -1)) ? XDC_RADT_INCONNU : XDC_RADT_CORRECT;*/
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = ((va_TypeStation == ERDTC_PEAGE) || (ValTO[vl_Chaussee] == -1)) ? XDC_RADT_TO_INCONNU : (float)(ValTO[vl_Chaussee] / vl_Nb_Voie_Ch[vl_Chaussee]);
				pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = ((va_TypeStation == ERDTC_PEAGE) || (ValTO[vl_Chaussee] == -1)) ? XDC_RADT_INCONNU : XDC_RADT_CORRECT;

				/*DEM298: modif calcul %PL */
				if (ValQ[vl_Chaussee] != -1)
				{
					val2 = (float)ValPL[vl_Chaussee];
					val3 = (float)ValQ[vl_Chaussee];

					val1 = val2 / val3;
					valint = val1;
					if ((val1 - valint) >= 0.5)
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Val = valint + 1;
					else
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Val = valint;

					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Indic = XDC_RADT_CORRECT;
				}
				else
				{
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
				}

				/*###########################################*/
				/* nouvelles regles de qualif*/
				/*###########################################*/

				/*RGV pas de valeur negative*/
				XZST_03EcritureTrace(XZSTC_INFO, "Valeur pas de n�gatif - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_VAL_NEG].Nom, pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_VAL_NEG].Valeur);
				if (pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_VAL_NEG].Valeur == XDC_OUI)
				{
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val < 0)
					{
						XZST_03EcritureTrace(XZSTC_INFO, "ACTIVATION : TO negatif");
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
					}

					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val < 0)
					{
						XZST_03EcritureTrace(XZSTC_INFO, "ACTIVATION : Q negatif");
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
					}
				}
				/*FIN  RGV pas de valeur negative*/

				/*RGV pas de debit excessif VRE*/
				XZST_03EcritureTrace(XZSTC_DEBUG1, "Valeur pas de debit excessif - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_DEB_EXCESS].Nom, pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_DEB_EXCESS].Valeur);
				XZST_03EcritureTrace(XZSTC_INFO, "Valeur pas de debit excessif - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_SEUIL_DEBIT_MAX].Nom, pg_Tab_REG_PARAMS[XDC_REG_SEUIL_DEBIT_MAX].Valeur);
				if (pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_DEB_EXCESS].Valeur == XDC_OUI)
				{
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val > 0) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val /
							 pg_Nb_Voie[va_Num_RADT][vl_Chaussee] >
						 pg_Tab_REG_PARAMS[XDC_REG_SEUIL_DEBIT_MAX].Valeur))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "ACTIVATION : Valeur pas de debit excessif");
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
					}
				}

				/*RGV pas de TO excessif VRE*/
				XZST_03EcritureTrace(XZSTC_DEBUG1, "Valeur pas de TO excessif - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_SEUIL_TO_INVALIDE].Nom, pg_Tab_REG_PARAMS[XDC_REG_SEUIL_TO_INVALIDE].Valeur);

				if (pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_TAUX_EXCESS].Valeur == XDC_OUI)
				{
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val > 0) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val >
						 pg_Tab_REG_PARAMS[XDC_REG_SEUIL_TO_INVALIDE].Valeur))
					{
						XZST_03EcritureTrace(XZSTC_INFO, "ACTIVATION : Valeur pas de TO excessif");
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
					}
				}

				/*RGV si Q=0 alors TO=0*/
				XZST_03EcritureTrace(XZSTC_DEBUG1, "Valeur si Q=0 alors TO=0 - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_QTO_NULS].Nom, pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_QTO_NULS].Valeur);

				if (pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_QTO_NULS].Valeur == XDC_OUI)
				{
					if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val == 0) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val > 0))
					{
						XZST_03EcritureTrace(XZSTC_INFO, "ACTIVATION : Valeur si Q=0 alors TO=0");
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
					}
				}

				/*RGV si TO=0 Q < seuil*/
				XZST_03EcritureTrace(XZSTC_INFO, "Valeur si TO=0 Q < seuil - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_QMAX_TONULS].Nom, pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_QMAX_TONULS].Valeur);
				XZST_03EcritureTrace(XZSTC_INFO, "Valeur si TO=0 Q < seuil - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_SEUIL_DEBIT_NUL].Nom, pg_Tab_REG_PARAMS[XDC_REG_SEUIL_DEBIT_NUL].Valeur);
				if (pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_QMAX_TONULS].Valeur == XDC_OUI)
				{
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val == 0)
					{
						if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val /
								pg_Nb_Voie[va_Num_RADT][vl_Chaussee] >
							pg_Tab_REG_PARAMS[XDC_REG_SEUIL_DEBIT_NUL].Valeur)
						{
							XZST_03EcritureTrace(XZSTC_INFO, "ACTIVATION : Valeur si TO=0 Q < seuil ");
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = -1;
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
						}
					}
				}

				/*RGV verifier nb Q nuls < seuil CYCLE */
				XZST_03EcritureTrace(XZSTC_INFO, "Valeur nb Q nuls < seuil - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_SEUIL_NUL].Nom, pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_SEUIL_NUL].Valeur);
				XZST_03EcritureTrace(XZSTC_INFO, "Valeur nb Q nuls < seuil - Utilisation parametre %s = %d",
									 pg_Tab_REG_PARAMS[XDC_REG_NBMAX_CYCLE_DEB_NUL].Nom, pg_Tab_REG_PARAMS[XDC_REG_NBMAX_CYCLE_DEB_NUL].Valeur);
				if (pg_Tab_REG_PARAMS[XDC_REG_ACTIVE_SEUIL_NUL].Valeur)
				{
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val == 0)
					{
						pg_Nb_Q_Nuls[va_Num_RADT][vl_Chaussee]++;
						if (pg_Nb_Q_Nuls[va_Num_RADT][vl_Chaussee] > pg_Tab_REG_PARAMS[XDC_REG_NBMAX_CYCLE_DEB_NUL].Valeur)
						{
							XZST_03EcritureTrace(XZSTC_WARNING, "Valeur nb Q nuls=%d", pg_Nb_Q_Nuls[va_Num_RADT][vl_Chaussee]);
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = -1;
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
						}
					}
					else
						pg_Nb_Q_Nuls[va_Num_RADT][vl_Chaussee] = 0;
				}

				/* Regle pas de donn�e le jour = donn�e la nuit ignor�e, verif du flag */
				XZST_03EcritureTrace(XZSTC_INFO, "Verif [Regle: pas de donn�e le jour = donn�e la nuit ignor�]");
				XZST_03EcritureTrace(XZSTC_INFO, "Verif [vl_Horo_en_mn #%d# > pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur #%d#]", vl_Horo_en_mn, pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur);
				XZST_03EcritureTrace(XZSTC_INFO, "Verif [vl_Horo_en_mn #%d# < pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur #%d#]", vl_Horo_en_mn, pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur);
				XZST_03EcritureTrace(XZSTC_INFO, "Verif [pg_Nb_Donnees_Jour[va_Num_RADT][vl_Chaussee] #%d# ==-1]", pg_Nb_Donnees_Jour[va_Num_RADT][vl_Chaussee]);
				if (((vl_Horo_en_mn > pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur) || (vl_Horo_en_mn < pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur)) && (pg_Nb_Donnees_Jour[va_Num_RADT][vl_Chaussee] == -1))
				{

					XZST_03EcritureTrace(XZSTC_INFO, "Je suis dans le IF [Regle: pas de donn�e le jour = donn�e la nuit ignor�]");
					XZST_03EcritureTrace(XZSTC_INFO, "pg_Nb_Donnees_Jour[va_Num_RADT] == (%d)", pg_Nb_Donnees_Jour[va_Num_RADT]);
					XZST_03EcritureTrace(XZSTC_INFO, "va_Num_RADT == (%d)", va_Num_RADT);

					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = -1;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = 255;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
					pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Indic = ((vl_Chaussee <= 2) && (ValPL[vl_Chaussee] != -1)) ? XDC_RADT_CORRECT : XDC_RADT_INCONNU;
				}
				/* FIN Regle pas de donn�e le jour = donn�e la nuit ignor�e */

				XZST_03EcritureTrace(XZSTC_FONCTION, "DEM476: calcul du debit chaussee, cas normal :vl_Nb_Voie_Ch[vl_Chaussee]=%d, vl_Chaussee=%d, ValQ.Val=%d",
									 vl_Nb_Voie_Ch[vl_Chaussee],
									 vl_Chaussee,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val);
			} /*fin else: cas normal */
		}	  /* fin vl_Nb_Voie_Ch[vl_Chaussee] > 0 */
		else
		{
			XZST_03EcritureTrace(XZSTC_FONCTION, "DEM476: Cas pas de voie sur chaussee:vl_Nb_Voie_Ch[vl_Chaussee]=%d, vl_Chaussee=%d",
								 vl_Nb_Voie_Ch[vl_Chaussee], vl_Chaussee);
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate = va_Horodate;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val = XDC_RADT_Q_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic = XDC_RADT_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val = XDC_RADT_V_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic = XDC_RADT_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val = XDC_RADT_TO_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic = XDC_RADT_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Val = XDC_RADT_PL_INCONNU;
			pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Indic = XDC_RADT_INCONNU;
		} /* fin else */

		XZST_03EcritureTrace(XZSTC_INFO, " station=%d, sens=%d, VITESS CHAUSSEE = %d", va_Num_RADT, vl_Chaussee, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val);
		XZST_03EcritureTrace(XZSTC_INFO, " DEBIT = %d", pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val);
		XZST_03EcritureTrace(XZSTC_DEBUG3, " POURCENTAGE = %d", ValPL[vl_Chaussee]);
		XZST_03EcritureTrace(XZSTC_DEBUG3, " POURCENTAGE POIDS LOURD PL = %d", pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValPL.Val);
		XZST_03EcritureTrace(XZSTC_INFO, "er_Calcul_Chaussee : Fin de Remplissage du tableau de structure de la chaussee");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : er_Calcul_Chaussee retourne OK");
	} /* fin boucle for */
	  /* Si la station possede une chaussee virtuelle appel de er_Calcul_RADT_Virtuelle */
	for (vl_i = 0; vl_i < ERDTC_NUM_MAX_STATION; vl_i++)
	{
		if (pg_Tab_RADT_Virtuelle[vl_i].NumStation1 == 0)
			continue;

		if ((pg_Tab_RADT_Virtuelle[vl_i].NumStation1 == va_Num_RADT) ||
			(pg_Tab_RADT_Virtuelle[vl_i].NumStation2 == va_Num_RADT) ||
			(pg_Tab_RADT_Virtuelle[vl_i].NumStation3 == va_Num_RADT))
		{
			/*
					XZST_03EcritureTrace( XZSTC_DEBUG1,"VIRT(%d) %d %d %d",
						va_Num_RADT,
						pg_Tab_RADT_Virtuelle[vl_i].NumStation1,
						pg_Tab_RADT_Virtuelle[vl_i].NumStation2,
						pg_Tab_RADT_Virtuelle[vl_i].NumStation3); */
			er_Calcul_RADT_Virtuelle(va_Num_RADT, ERDTC_6MINUTE, va_Index);
			break;
		}
	}
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Recherche du Contexte des seuils pour le calcul des niveaux de service
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Contexte()

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
* Recherche du contexte des seuils pour le calcul des niveaux de service
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ : er_Contexte";

	XDY_FichEquext pl_PathEtFicC = "";
	XDY_Texte pl_Ligne = "";
	int vl_Fd = 0;
	int vl_ValRet = 0;
	char pl_Date[11] = "";
	char pl_Heure[9] = "";
	char pl_Horodate[20] = "";
	char pl_Horodate_Chgt[20] = "";
	XDY_Horodate pl_Date_Sec = 0.0;
	char vl_Contexte[ERDTC_NB_STATION] = "";
	XDY_Entier vl_NbParam = 0;
	time_t vl_Time;
	struct tm *pl_Date_Courante = NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s ", version);

	vg_Date_Changement = vg_Date_Actuelle + 300.0;
	vg_ContexteSeuils = (int)NULL;

	/*A Ouverture du fichier du calendrier des contextes */
	sprintf(pl_PathEtFicC, "%s/%s", ERDTC_PATH_FIC_SNT, ERDTC_FIC_CAL_DFT);

	if ((vl_Fd = open(pl_PathEtFicC, O_RDONLY)) == -1)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : fichier %s. introuvable", version, pl_PathEtFicC);
		return (XDC_NOK);
	}

	/*A Lecture du fichier du calendrier des contextes */
	while (ex_LireLigneFich(vl_Fd, pl_Ligne, sizeof(pl_Ligne)) != XZEXC_EOF)
	{
		if (*pl_Ligne != ERDTC_COMMENTAIRE)
		{
			/*B Extraction des seuils de niveau de trafic et Remplissage du tableau des seuils de trafic */
			if ((vl_NbParam = sscanf(pl_Ligne, "%s %s %s", pl_Date, pl_Heure, vl_Contexte)) == 3)
			{
				if ((vl_ValRet = XZSM_11ConversionHorodate(vg_Date_Actuelle, &pl_Horodate_Chgt)) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "Pb recuperation du Nb Secondes");
					return (XDC_NOK);
				}

				if (!strncmp(pl_Horodate_Chgt, pl_Date, 10))
				{
					sprintf(pl_Horodate, "%s %s:00", pl_Date, pl_Heure);
					if ((vl_ValRet = XZSM_13ConversionHorodateSec(pl_Horodate, &pl_Date_Sec)) != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "Pb recuperation du Nb Secondes");
						return (XDC_NOK);
					}
					if (vg_Date_Actuelle >= pl_Date_Sec)
					{
						switch (vl_Contexte[0])
						{
						case ERDTC_JOUR_SEMAIME:
							vg_ContexteSeuils = ERDTC_SEMAIME;
							break;
						case ERDTC_JOUR_WEEKEND:
							vg_ContexteSeuils = ERDTC_WEEKEND;
							break;
						case ERDTC_JOUR_FERIE:
							vg_ContexteSeuils = ERDTC_FERIE;
							break;
						}
					}
				}
			}
		}
	}

	/*A Fermeture du fichier XDF_CALENDRIER_CONTEXTE */
	XZSS_13FermerFichier(vl_Fd);

#ifdef _HPUX_SOURCE
	if (vg_ContexteSeuils == NULL)
#else
	if (vg_ContexteSeuils == 0)
#endif
	{

		// SAE_191: Modif chantier date
		/* Ici on cherche a determiner le jour de la semaine donc pas d'encapsulation necessaire */
		time(&vl_Time);
		pl_Date_Courante = localtime(&vl_Time);

		switch (pl_Date_Courante->tm_wday)
		{
		case 6: /* Samedi */
		case 0: /* Dimanche */
			vg_ContexteSeuils = ERDTC_WEEKEND;
			break;
		default: /* 1 a 5 : Lundi a Vendredi */
			vg_ContexteSeuils = ERDTC_SEMAIME;
			break;
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne OK avec Contexte = %d ", version, vg_ContexteSeuils);
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Calcul des niveaux de service
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Niveau_Service(XDY_Eqt va_Num_RADT,
					  XDY_Mot va_TypeStation,
					  XDY_Entier va_Index,
					  XDY_Horodate va_Horodate,
					  XDY_Entier va_Periode)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT : Numero de la station RADT
*   va_TypeStation : Type de la station
*   va_Index : Numero de l'index du tableau de valeurs
*   va_Horodate : Horodate
*   va_Periode : Periode des mesures pour calcul
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
* Calcul des donnees par chaussee en fonction des donnees par voies pour une station RADT
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ : er_Niveau_Service";

	XDY_Entier vl_Anc_Niveau[ERDTC_NB_CHAUS_S_COUR];
	XDY_ENiveau_Service vl_Niveau;
	XDY_Datagroup pl_DG_Niv;
	XDY_Horodate vl_Horodate = 0.0;
	XDY_Entier vl_Contexte = 0;
	XDY_Entier vl_retcode = 0;
	XDY_Entier vl_i = 0;
	XDY_Sens vl_Sens = 0;
	XDY_Texte pl_Msg = "";
	XZEXT_MSG_SOCKET pl_MsgTDC = "";
	int vl_Cpt = 0;
	int vl_EtatVoie[ERDTC_NB_CHAUS_S_COUR];
	ERDT_DONNEES_RADT *pl_ListeRADT;
	size_t vl_LgrMsgTDC;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s : RADT = %d  Type = %d  Ind. = %d  Hor. = %lf",
						 version, va_Num_RADT, va_TypeStation, va_Index, va_Horodate);

	for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
		if (pl_ListeRADT->ConfRADT.NumeroStation == va_Num_RADT)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "IN : %sNiveau avt : RADT = %d  Type = %d  Ind. = %d  Hor. = %lf Etat=%d/%d/%d",
								 version, va_Num_RADT, va_TypeStation, va_Index, va_Horodate, pl_ListeRADT->Etat_Service, pl_ListeRADT->Etat_Service & XDC_EQT_HORS_SRV, pl_ListeRADT->Etat_Service & XDC_EQT_DESACTIVE);
			if ((pl_ListeRADT->Etat_Service & XDC_EQT_HORS_SRV) || (pl_ListeRADT->Etat_Service & XDC_EQT_DESACTIVE))
				return (XDC_OK);
			break;
		}

	XZST_03EcritureTrace(XZSTC_WARNING, "IN : %sNiveau : RADT = %d  Type = %d  Ind. = %d  Hor. = %lf Etat=%d",
						 version, va_Num_RADT, va_TypeStation, va_Index, va_Horodate, pl_ListeRADT->Etat_Service);

	/*A Recherche du contexte dans le calendrier : Appel de la fonction er_Contexte */
	if ((vg_Date_Actuelle >= vg_Date_Changement) || (vg_Date_Changement == 0.0))
		if ((vl_retcode = er_Contexte()) != XDC_OK)
			XZST_03EcritureTrace(XZSTC_WARNING, "%s : Appel er_Contexte retourne %d ", version, vl_retcode);

	/*A Recherche de l'index du contexte pour le tableau des seuils */
	if (vg_Contexte == XDC_JOUR)
		vl_Contexte = 2 * (vg_ContexteSeuils - 1);
	else
		vl_Contexte = 2 * (vg_ContexteSeuils - 1) + 1;

	XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : Contexte = %d ", version, vl_Contexte);

	if (!vg_Mac_Ext_Ouest && (pl_ListeRADT != NULL) && !(pl_ListeRADT->Etat_Service))
	{
		/*A Envoi du contexte a TETDC */
		vl_Cpt = sprintf(pl_MsgTDC, "%1d%1d%", ERDTC_MSG_CTXT, vl_Contexte);
		strcat(pl_MsgTDC + vl_Cpt + 1, XZEXC_FIN_MSG_SOCK);
		vl_LgrMsgTDC = (size_t)vl_Cpt + 1 + strlen(XZEXC_FIN_MSG_SOCK);
		sem_wait(&vg_semaphore);
		if ((vl_retcode = (XDY_Entier)write(vg_SockTDC, pl_MsgTDC, vl_LgrMsgTDC)) <= 0)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "%s :  Erreur envoi message contexte socket TETDC ", version);
		}
		else
			XZST_03EcritureTrace(XZSTC_INFO, "%s : Envoi TETDC Mes = <%s> Lg = %d   Lg Env. = %d ", version, pl_MsgTDC, vl_LgrMsgTDC, vl_retcode);
		sem_post(&vg_semaphore);
	}

	/*A Si la station n'a pas de sens Sud ou Nord => Sortie */
	if ((va_TypeStation == ERDTC_PEAGE) || (va_TypeStation == ERDTC_ECHANGEUR) || (va_TypeStation == ERDTC_PEAG_ECHA))
	{
		XZST_03EcritureTrace(XZSTC_INFO, "%s :  Station de type %d", version, va_TypeStation);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne XDC_OK", version, va_TypeStation);
		return (XDC_OK);
	}

	switch (va_Periode)
	{
	case ERDTC_MINUTE:
		/*A Calcul du niveau de service par seuils, avec une hysteresis pour garantir la stabilite dans le temps
			 Calcul par rapport a TO, V et Q  du niveau de service par chaussee */
		for (vl_i = 1; vl_i <= ERDTC_NB_CHAUS_S_COUR; vl_i++)
		{
			vl_EtatVoie[vl_i - 1] = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Voie;
			if (vl_EtatVoie[vl_i - 1] != XDC_VOIE_INCONNUE)
			{
				XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : NivTO = %d\tNivV = %d\tNivQ = %d\tRADT = %d\n\tSTO1 = %d\tSTO2 = %d\tSTO3 = %d\tSTO4 = %d\tSV1 = %d\tSV2 = %d\tSV3 = %d\tSV4 = %d\tSQ1 = %d\tSQ2 = %d", version,
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0],
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1],
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2],
									 va_Num_RADT,
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][0],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][1],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][2],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][3],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][4],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][5],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][6],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][7],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][8],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][9]);

				/*B Recherche de l'ancien niveau de service global */
				vl_Anc_Niveau[vl_i - 1] = MIN(pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0], pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1]);
				vl_Anc_Niveau[vl_i - 1] = MIN(vl_Anc_Niveau[vl_i - 1], pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2]);

				if ((pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Indic != XDC_RADT_INCONNU) &&
					(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Indic != XDC_RADT_RECONST))

				{
					/*B Si le Niveau precedent est fluide et ValTO > Seuil F->PS =>  le Niveau est presature */
					if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_FLUIDE) &&
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][0]))
					{
						pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] = XZERC_NIV_PRESATURE;
					}
					else
					{
						/*B Si le Niveau precedent est presature ou fluide et ValTO > Seuil PS->S =>  le Niveau est sature */
						if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_PRESATURE ||
							 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_FLUIDE) &&
							pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][1])
						{
							pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] = XZERC_NIV_SATURE;
						}
						else
						{
							/*B Si le Niveau precedent est sature et ValTO < Seuil S ->PS =>  le Niveau est presature */
							if (pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_SATURE &&
								pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][2])
							{
								pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] = XZERC_NIV_PRESATURE;
							}
							else
							{
								/*B Si le Niveau precedent est sature ou presature et ValTO < Seuil PS->F =>  le Niveau est fluide */
								if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_SATURE ||
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_PRESATURE) &&
									pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][3])
								{
									pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] = XZERC_NIV_FLUIDE;
								}
							}
						}
					}
				}

				if ((pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Indic != XDC_RADT_INCONNU) &&
					(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Indic != XDC_RADT_RECONST))
				{
					/*B Si le Niveau precedent est fluide et ValV < Seuil F->PS => le Niveau est presature */
					if (pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_FLUIDE &&
						pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][4])
					{
						pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] = XZERC_NIV_PRESATURE;
					}
					else
					{
						/*B Si le Niveau precedent est presature ou fluide et ValV < Seuil PS->S =>  le Niveau est sature */
						if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_PRESATURE ||
							 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_FLUIDE) &&
							pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][5])
						{
							pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] = XZERC_NIV_SATURE;
						}
						else
						{
							/*B Si le Niveau precedent est sature et ValV > Seuil S->PS  =>  le Niveau est presature */
							if (pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_SATURE &&
								pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][6])
							{
								pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] = XZERC_NIV_PRESATURE;
							}
							else
							{
								/*B Si le Niveau precedent est presature ou sature et ValV > Seuil PS->F =>  le Niveau est fluide */
								if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_SATURE ||
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_PRESATURE) &&
									pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][7])
								{
									pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] = XZERC_NIV_FLUIDE;
								}
							}
						}
					}
				}
				if ((pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Indic != XDC_RADT_INCONNU) &&
					(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Indic != XDC_RADT_RECONST))
				{
					/*B Si le Niveau precedent est fluide ou sature et ValQ > Seuil FouS->PS =>  le Niveau est presature */
					if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] == XZERC_NIV_SATURE ||
						 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] == XZERC_NIV_FLUIDE) &&
						(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Val) > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][8])
					{
						pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] = XZERC_NIV_PRESATURE;
					}
					else
					{
						/*B Si le Niveau precedent est presature et ValQ < Seuil PS->FouS =>  le Niveau est fluide ou sature */
						if (pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] == XZERC_NIV_PRESATURE &&
							(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Val) < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][9])
						{
							pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] = XZERC_NIV_SATURE;
						}
					}
				}
			}
		}
		break;

	case ERDTC_6MINUTE:
		/*A Calcul du niveau de service par seuils, avec une hysteresis pour garantir la stabilite dans le temps
			 Calcul par rapport a TO, V et Q  du niveau de service par chaussee */
		for (vl_i = 1; vl_i <= ERDTC_NB_CHAUS_S_COUR; vl_i++)
		{
			vl_EtatVoie[vl_i - 1] = pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Voie;
			if (vl_EtatVoie[vl_i - 1] != XDC_VOIE_INCONNUE)
			{
				XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : NivTO = %d\tNivV = %d\tNivQ = %d\tRADT = %d\n\tSTO1 = %d\tSTO2 = %d\tSTO3 = %d\tSTO4 = %d\tSV1 = %d\tSV2 = %d\tSV3 = %d\tSV4 = %d\tSQ1 = %d\tSQ2 = %d", version,
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0],
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1],
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2],
									 va_Num_RADT,
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][0],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][1],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][2],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][3],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][4],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][5],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][6],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][7],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][8],
									 pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][9]);

				/*B Recherche de l'ancien niveau de service global */
				vl_Anc_Niveau[vl_i - 1] = MIN(pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0], pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1]);
				vl_Anc_Niveau[vl_i - 1] = MIN(vl_Anc_Niveau[vl_i - 1], pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2]);

				if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Indic != XDC_RADT_INCONNU) &&
					(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Indic != XDC_RADT_RECONST))

				{
					/*B Si le Niveau precedent est fluide et ValTO > Seuil F->PS =>  le Niveau est presature */
					if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_FLUIDE) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][0]))
					{
						pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] = XZERC_NIV_PRESATURE;
					}
					else
					{

						/*B Si le Niveau precedent est presature ou fluide et ValTO > Seuil PS->S =>  le Niveau est sature */
						if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_PRESATURE ||
							 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_FLUIDE) &&
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][1])
						{
							pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] = XZERC_NIV_SATURE;
						}
						else
						{
							/*B Si le Niveau precedent est sature et ValTO < Seuil S ->PS =>  le Niveau est presature */
							if (pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_SATURE &&
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][2])
							{
								pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] = XZERC_NIV_PRESATURE;
							}
							else
							{
								/*B Si le Niveau precedent est sature ou presature et ValTO < Seuil PS->F =>  le Niveau est fluide */
								if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_SATURE ||
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] == XZERC_NIV_PRESATURE) &&
									pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][3])
								{
									pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0] = XZERC_NIV_FLUIDE;
								}
							}
						}
					}
				}

				if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Indic != XDC_RADT_INCONNU) &&
					(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Indic != XDC_RADT_RECONST))
				{
					/*B Si le Niveau precedent est fluide et ValV < Seuil F->PS => le Niveau est presature */
					if (pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_FLUIDE &&
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][4])
					{
						pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] = XZERC_NIV_PRESATURE;
					}
					else
					{
						/*B Si le Niveau precedent est presature ou fluide et ValV < Seuil PS->S =>  le Niveau est sature */
						if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_PRESATURE ||
							 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_FLUIDE) &&
							pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][5])
						{
							pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] = XZERC_NIV_SATURE;
						}
						else
						{
							/*B Si le Niveau precedent est sature et ValV > Seuil S->PS  =>  le Niveau est presature */
							if (pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_SATURE &&
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][6])
							{
								pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] = XZERC_NIV_PRESATURE;
							}
							else
							{
								/*B Si le Niveau precedent est presature ou sature et ValV > Seuil PS->F =>  le Niveau est fluide */
								if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_SATURE ||
									 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] == XZERC_NIV_PRESATURE) &&
									pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][7])
								{
									pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1] = XZERC_NIV_FLUIDE;
								}
							}
						}
					}
				}

				if ((pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Indic != XDC_RADT_INCONNU) &&
					(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Indic != XDC_RADT_RECONST))
				{
					/*B Si le Niveau precedent est fluide ou sature et ValQ > Seuil FouS->PS =>  le Niveau est presature */
					if ((pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] == XZERC_NIV_SATURE ||
						 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] == XZERC_NIV_FLUIDE) &&
						(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Val) > pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][8])
					{
						pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] = XZERC_NIV_PRESATURE;
					}
					else
					{
						/*B Si le Niveau precedent est presature et ValQ < Seuil PS->FouS =>  le Niveau est fluide ou sature */
						if (pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] == XZERC_NIV_PRESATURE &&
							(pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Val) < pg_Seuils[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][vl_Contexte][9])
						{
							pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2] = XZERC_NIV_SATURE;
						}
					}
				}
			}
		}
		break;

	default:
		return (XDC_NOK);

	} /* fin switch */

	for (vl_i = 1; vl_i <= ERDTC_NB_CHAUS_S_COUR; vl_i++)
	{
		if (vl_EtatVoie[vl_i - 1] != XDC_VOIE_INCONNUE)
		{
			/*A Calcul du niveau de trafic global a partir du niveau de trafic calcule pour chaque variable TO, V et Q Niveau = inf ( NivTO,NivV,NivQ) */
			vl_Niveau.NivService = MIN(pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0], pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1]);
			vl_Niveau.NivService = MIN(vl_Niveau.NivService, pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2]);
			vl_Niveau.Horodate = va_Horodate;
			vl_Niveau.Numero = va_Num_RADT;
			vl_Niveau.Sens = vl_i;

			XZST_03EcritureTrace(XZSTC_INFO, "%s : NivTO = %d\tNivV = %d\tNivQ = %d\tNIV = %d\tAncNIV = %d\tHoro = %lf\tRADT = %d\tSens = %d", version,
								 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0],
								 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1],
								 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2],
								 vl_Niveau.NivService,
								 vl_Anc_Niveau[vl_i - 1],
								 vl_Niveau.Horodate,
								 va_Num_RADT,
								 vl_Niveau.Sens);

			/*A Envoi des niveaux de service pour la RADT a l'IHM pour les chaussees de la RADT */
			/*B Envoi de XDM_ENiveau_Service dans XDM_ENiveau_Service_<NomSite> */

			XZST_03EcritureTrace(XZSTC_INFO, "Niveau Service : Envoi du message XDM_ENiveau_Service_<NomSite> en fournissant les valeur calculees");

			/*B Construction du DatGroup XDG_ENiveau_Service_<NomSite> */
			/*sprintf ( pl_DG_Niv, "%s_%s", XDG_ENIVEAU_SERVICE, vg_NomSite );*/

			/*SITEGESTION DEM748*/
			sprintf(pl_DG_Niv, "%s_%s", XDG_ENIVEAU_SERVICE, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion));

			/*B Envoi du message XDM_ENiveau_Service en fournissant les valeur recues */
			vl_Sens = vl_i;
			XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Envoi %s", version, pl_DG_Niv);

			if (!TipcSrvMsgWrite(pl_DG_Niv, XDM_IdentMsg(XDM_ENiveau_Service), XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_Niveau.Horodate, T_IPC_FT_INT2, va_Num_RADT, T_IPC_FT_INT2, vl_Sens, T_IPC_FT_INT2, vl_Niveau.NivService, NULL))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "er_calcul_niveau : Envoi message XDM_ENiveau_Service non effectue");
			}

			/*B Envoi du message */
			TipcSrvFlush();

			if (!vg_Mac_Ext_Ouest && (pl_ListeRADT != NULL) && !(pl_ListeRADT->Etat_Service))
			{
				/*B Si le niveau de service a changer => Envoi d'une alerte */
				sprintf(pl_Msg, "Changement Niveau Service Station RADT %d Sens %d", va_Num_RADT, vl_Sens);

				XZST_03EcritureTrace(XZSTC_DEBUG1, "boucle station=%d, alerte=%s ", va_Num_RADT, pl_ListeRADT->ConfRADT.Alertes);

				if (vl_Niveau.NivService != vl_Anc_Niveau[vl_i - 1])
				{
					if (strcmp(pl_ListeRADT->ConfRADT.Alertes, "0") != 0)
					{
						XZST_03EcritureTrace(XZSTC_DEBUG1, "alerte station=%d, alerte=%s ", va_Num_RADT, pl_ListeRADT->ConfRADT.Alertes);
						ex_env_alerte(va_Horodate, XDC_EQT_RAD, va_Num_RADT, XZAAC_NIV_RADT, vg_Mode_Fonct,
									  pl_ListeRADT->ConfRADT.SiteGestion, pl_Msg,
									  NULL);
						XZST_03EcritureTrace(XZSTC_DEBUG2, "Alertes_RADT %s : NivTO = %d\tNivV = %d\tNivQ = %d\tNIV = %d\tAncNIV = %d\tHoro = %lf\tRADT = %d\tSens = %d\tnumero_station_radt = %s", version,
											 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][0],
											 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][1],
											 pg_Niveau[pg_Tab_RADT[va_Num_RADT]][vl_i - 1][2],
											 vl_Niveau.NivService,
											 vl_Anc_Niveau[vl_i - 1],
											 vl_Niveau.Horodate,
											 va_Num_RADT,
											 vl_Niveau.Sens,
											 pl_ListeRADT->ConfRADT.NumeroStation);
					}
				}
				XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : Horo = %lf\tRADT = %d\tSens = %d\tNiveau = %d\n", version, vl_Niveau.Horodate, va_Num_RADT, vl_Sens, vl_Niveau.NivService);
				XZST_03EcritureTrace(XZSTC_DEBUG3, " RADT = %d\tSens = %d\tNiveau = %d\tQ = %d\tV = %d\tTO = %d\tPL = %d", va_Num_RADT, vl_Sens, vl_Niveau.NivService,
									 pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValQ.Val,
									 pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValV.Val,
									 pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][0][va_Index].Mesures.ValTO.Val,
									 pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[va_Num_RADT]][vl_i][1][va_Index].Mesures.ValPL.Val);
			}
		}
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne XDC_OK", version);
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi des mesures 6 minutes par voie et par chaussee
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Fichier_Mesures(XDY_Eqt va_Num_RADT, XDY_Entier va_Index, XDY_Horodate va_Horodate)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT : Numero de la station RADT
*   va_Index : Numero de l'index du tableau de valeurs
*   va_Horodate : Horodate de stockage
*
* ARGUMENTS EN SORTIE :
*
*   pa_Mesure   : Mesure Chaussee
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
* Envoi des mesures 6 minutes par voie et par chaussee
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Fichier_Mesures";

	XDY_Mot vl_Chaussee = 0;
	XDY_Mot vl_Voie = 0;
	char pl_Texte[20] = "";
	FILE *vl_Fd;
	XDY_Texte pl_PathEtFic;
	int vl_jourSemaine = 0;
	time_t vl_time = 0;
	XDY_Horodate vl_horodateSec = 0.0;
	int vl_jour = 0, vl_mois = 0, vl_annee = 0;
	ERDT_DONNEES_PtMes *pl_ListePtMes;

	/*A Si le fichier stock_mesures n'existe pas => sortie */
	if (!vg_mode_simu)
	{
		if (access("stock_mesures", F_OK))
			return (XDC_OK);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN %s : RADT = %d  Index = %d", version, va_Num_RADT, va_Index);

	/*A Recherche de l'horodate pour la construction du nom du fichier des stockge des mesures */
	/*time ( &vl_time );
	vl_horodateSec = (XDY_Horodate) vl_time;  remplacer par XZSM_07 XZAE_191*/
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_horodateSec);
	XZSM_11ConversionHorodate(vl_horodateSec, &pl_Texte);
	sscanf(pl_Texte, "%02d/%02d/%04d", &vl_jour, &vl_mois, &vl_annee);

	sprintf(pl_PathEtFic, "%s/%s_R%d.%02d_%02d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_SAUV_QVTO, va_Num_RADT, vl_jour, vl_mois);
	vl_Fd = fopen(pl_PathEtFic, "a+");

	fprintf(vl_Fd, "%s\n", "###############################################################################");
	XZSM_11ConversionHorodate(va_Horodate, &pl_Texte);
	for (vl_Chaussee = 1; vl_Chaussee < ERDTC_NB_CHAUSSEE; (vl_Chaussee == 4) ? vl_Chaussee += 3 : vl_Chaussee++)
		for (vl_Voie = (ERDTC_NB_VOIE_CHAUS - 1); vl_Voie > -1; vl_Voie--)
		{
			/* Envoi valeur nulles pour voie neutralise */
			for (pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant)
				if ((pl_ListePtMes->ConfPtMes.NumeroStation == va_Num_RADT) &&
					(pl_ListePtMes->ConfPtMes.Voie == vl_Voie) &&
					(pl_ListePtMes->ConfPtMes.Sens == vl_Chaussee))
				{
					if ((pl_ListePtMes->EtatVoie == XDC_VOIE_BLOQUEE) ||
						(pl_ListePtMes->EtatVoie == XDC_VOIE_NEUTRALISEE) ||
						(pl_ListePtMes->EtatVoie == XDC_VOIE_BLOQUEE_NEUTRALISEE))
						fprintf(vl_Fd, "%s\t%d\t%d\t%d\t%d\t0\t%d\t0\t%d\t0\t%d\t0\t%d\n",
								pl_Texte,
								va_Num_RADT, ERDTC_6MINUTE, vl_Chaussee, vl_Voie,
								XDC_VOIE_NEUTRALISEE,
								XDC_VOIE_NEUTRALISEE,
								XDC_VOIE_NEUTRALISEE,
								XDC_VOIE_NEUTRALISEE);

					break;
				}
			if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie != XDC_VOIE_INCONNUE)
			{
				fprintf(vl_Fd, "%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
						pl_Texte,
						va_Num_RADT, ERDTC_6MINUTE, vl_Chaussee, vl_Voie,
						pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic);

				XZST_03EcritureTrace(XZSTC_INFO, "Fichier_mMesures pour envoi: vl_Chaussee = %d\t, vl_Voie=%d\t, ValQ.Val=%d\t, ValQ.Indic=%d\t,ValV.Val=%d\t,ValV.Indic=%d\t,ValTO.Val=%d\t,ValTO.Indic=%d\t,ValPL.Val=%d\t,ValPL.Indic=%d\t",
									 vl_Chaussee, vl_Voie, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic);
			}

		} /* fin for vl_Voie */
	fclose(vl_Fd);
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s", version);
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Stockage des mesures 6 minutes par voie et par chaussee dans un fichier : Mes_Traitees_R<station>
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Stockage_Mesures(XDY_Eqt va_Num_RADT, XDY_Entier va_Index, XDY_Horodate va_Horodate, XDY_Entier va_Mesures_Valides)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT : Numero de la station RADT
*   va_Index : Numero de l'index du tableau de valeurs
*
* ARGUMENTS EN SORTIE :
*
*   pa_Mesure   : Mesure Chaussee
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
* Stockage des mesures 6 minutes par voie et par chaussee dans un fichier : Mes_Traitees_R<station>
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Stockage_Mesures";

	XDY_Mot vl_Chaussee = 0;
	XDY_Mot vl_Voie = 0;
	FILE *vl_Fd;
	XDY_Texte pl_PathEtFic = "";
	XDY_Texte pl_PathEtFicLect = "";
	ERDT_DONNEES_PtMes *pl_ListePtMes;
	char vl_heure_tab[3];
	char vl_minute_tab[3];
	int vl_Horo_en_mn = 0;
	int vl_minute = 0;
	int vl_heure = 0;
	XZSMT_Horodate pl_Horodatedecompo;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN %s : RADT = %d  Index = %d", version, va_Num_RADT, va_Index);

	/*A Construction du nom du fichier */
	sprintf(pl_PathEtFic, "%s/%s_W%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_STOCK_QVTO, va_Num_RADT);
	vl_Fd = fopen(pl_PathEtFic, "a+");
	if (vl_Fd == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s Impossible de creer le fichier <%s>", version, pl_PathEtFic);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne 0", version);
		return (XDC_NOK);
	}

	for (vl_Chaussee = 1; vl_Chaussee < ERDTC_NB_CHAUSSEE; (vl_Chaussee == 4) ? vl_Chaussee += 3 : vl_Chaussee++)
		for (vl_Voie = (ERDTC_NB_VOIE_CHAUS - 1); vl_Voie > -1; vl_Voie--)
		{
			/* Envoi valeur nulles pour voie neutralise */
			for (pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant)
			{
				/*rajout condition pour pb insert Q=0*/
				/*if ( (pl_ListePtMes->ConfPtMes.NumeroStation == va_Num_RADT) && (pl_ListePtMes->ConfPtMes.Voie == vl_Voie) && (pl_ListePtMes->ConfPtMes.Sens == vl_Chaussee) )
					{
					   if ( (pl_ListePtMes->EtatVoie == XDC_VOIE_BLOQUEE) ||
							(pl_ListePtMes->EtatVoie == XDC_VOIE_NEUTRALISEE) ||
							(pl_ListePtMes->EtatVoie == XDC_VOIE_BLOQUEE_NEUTRALISEE) )
					  fprintf (vl_Fd , "%lf\t%d\t%d\t0\t%d\t0\t%d\t0\t%d\t0\t%d\n",
							   va_Horodate, vl_Chaussee, vl_Voie,
								   XDC_RADT_CORRECT,
								   XDC_RADT_CORRECT,
								   XDC_RADT_CORRECT,
								   XDC_RADT_CORRECT );

					   break;
					}*/
			}

			/*DEM476: si (( 07.30 < Heure < 22.00) et (Q = 0)) alors ne pas creer d'enregistrement en base pour la voie */
			/* Convertir l'horodate recu en sec en JJ/MM/AAAA HH:MM:SS */
			XZSM_11ConversionHorodate(va_Horodate, &pl_Horodatedecompo);
			strncpy(vl_heure_tab, &pl_Horodatedecompo[11], 2);
			vl_heure_tab[2] = '\0';
			vl_heure = atoi(vl_heure_tab);
			strncpy(vl_minute_tab, &pl_Horodatedecompo[14], 2);
			vl_minute_tab[2] = '\0';
			vl_minute = atoi(vl_minute_tab);

			/*07.30 = 450 minutes et 22.00 = 1320 minutes */
			vl_Horo_en_mn = ((vl_heure * 60) + vl_minute);
			XZST_03EcritureTrace(XZSTC_DEBUG1, "PNI station=%d, le debit recu est: Q[%d,%d] = %d", 
												va_Num_RADT, 
												vl_Chaussee, 
												vl_Voie, 
												pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);
			if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val != 0)
			{

				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie != XDC_VOIE_INCONNUE)
				{
					/*si sens 1 ou 2 et voie=0 et Q<50 alors pas d'insert en base */
					/*if ( ((vl_Chaussee == 1) || (vl_Chaussee == 2)) && (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val >=  50.0))*/
					if (((vl_Chaussee != 1) && (vl_Chaussee != 2)) || (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val >= 50.0) || (vl_Voie != 0) || ((vl_Horo_en_mn <= pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) || (vl_Horo_en_mn >= pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur))) /*DEM590*/
																																																																																			  /*DEM 612 : Suppression des Q<50 uniquement dans la plage horaire */
																																																																																			  /*DEM 696 :Seules Les donn�es Q(chauss�e) RADT des sens 1 et 2 doivent �tre filtr�es sur Q(chauss�e)<50 */
					{
						fprintf(vl_Fd, "%.0lf\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
								va_Horodate, 
								vl_Chaussee, 
								vl_Voie, 
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val, 
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic, 
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val, 
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic, 
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val, 
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic, 
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val, 
								pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic);
						XZST_03EcritureTrace(XZSTC_DEBUG1, "PNI donnees ecrites");
					}
					else
						XZST_03EcritureTrace(XZSTC_DEBUG1, "PNI station=%d, PAs de mise en fic: Q[%d,%d] = %d", va_Num_RADT, vl_Chaussee, vl_Voie, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);
				}
				else
					XZST_03EcritureTrace(XZSTC_DEBUG1, "PNI XDC_VOIE_INCONNUE");
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_INFO, "DEM476 station=%d, le debit recu est nul: Q[%d] = %d", va_Num_RADT, vl_Chaussee, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val);
				/*verifier maintenant que l'on ne se trouve pas dans le tranche 07.30 - 22 heures pour pouvoir transmettre */
				if ((vl_Horo_en_mn <= pg_Tab_REG_PARAMS[XDC_REG_HEURE_DEB_JOUR].Valeur) || (vl_Horo_en_mn >= pg_Tab_REG_PARAMS[XDC_REG_HEURE_FIN_JOUR].Valeur))
				{
					XZST_03EcritureTrace(XZSTC_INFO, "DEM476 le debit est nul mais on est bien dans la tranche avant 07.30 ou apres 22 heures");
					if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie != XDC_VOIE_INCONNUE)
					{
						fprintf(vl_Fd, "%.0lf\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
								va_Horodate, vl_Chaussee, vl_Voie, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValPL.Indic);
						XZST_03EcritureTrace(XZSTC_DEBUG1, "PNI donnees ecrites");
					}
					else
						XZST_03EcritureTrace(XZSTC_DEBUG1, "PNI XDC_VOIE_INCONNUE");
				}
			}
		}
	fclose(vl_Fd);
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne 0", version);

	/*A Recopie du fichier ecriture si pas de fichier lecture */
	sprintf(pl_PathEtFicLect, "%s/%s_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_STOCK_QVTO, va_Num_RADT);
	if (access(pl_PathEtFicLect, F_OK))
		rename(pl_PathEtFic, pl_PathEtFicLect);

	/*B Ecriture de l'horodate des dernieres mesures recues dans un fichier si pas mesures extrapolees*/
	if (va_Mesures_Valides == XDC_VRAI)
		er_Maj_Fichier_heure(va_Num_RADT, va_Horodate);

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *
 *  Mise a jour du fichier derniere heure d'acquisition
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int er_Maj_Fichier_heure(XDY_Eqt va_Num_RADT, XDY_Horodate va_Horodate)

/*
 * ARGUMENTS EN ENTREE :
 *
 *   va_Num_RADT : Numero de la station RADT
 *   va_Horodate	: horodate a mettre a jour
 *
 * ARGUMENTS EN SORTIE :
 *
 *
 * CODE RETOUR :
 *   XDC_OK	succes
 *   XDC_NOK	echec
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *
 *
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Maj_Fichier_heure ";
	FILE *vl_Fd;
	XDY_Texte pl_PathEtFic = "";
	XZSMT_Horodate vl_heure_string_his = "";
	ERDT_DONNEES_RADT *pl_ListeRADT;

	/*A Stockage de la derniere heure d'acquisition */
	for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
		if (pl_ListeRADT->ConfRADT.NumeroStation == va_Num_RADT)
		{
			pl_ListeRADT->Don_HisRADT.Horodate_Histo = va_Horodate;
			break;
		}
	XZSM_11ConversionHorodate(va_Horodate, &vl_heure_string_his);
	XZST_03EcritureTrace(XZSTC_INFO, "%s : MAJ HR St %d :: Hr = <%s>", version, va_Num_RADT, vl_heure_string_his);
	sprintf(pl_PathEtFic, "%s/%s_%s_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_HEUR_QVTO, vg_NomMachine, va_Num_RADT);
	if ((vl_Fd = fopen(pl_PathEtFic, "w+")) == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : fichier %s. introuvable", version, pl_PathEtFic);
		return (XDC_NOK);
	}
	else
	{
		fprintf(vl_Fd, "%s \n", vl_heure_string_his);
		fclose(vl_Fd);
	}

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi des mesures 6 minutes par voie et par chaussee (Lecture du fichier)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Envoi_Mesure(XDY_Eqt va_Num_RADT, XDY_Booleen va_Val_Suivante)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT : Numero de la station RADT
*   va_Val_Suivante : Booleen pour designer s'il faut lire la valeur suivante ou renvoyer la derniere
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
* Envoi des mesures 6 minutes par voie et par chaussee (Lecture du fichier)
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Envoi_Mesure";

	XDY_Datagroup pl_DG = "";
	char pl_Ligne[250] = "";
	XDY_Horodate vl_Horodate;
	int vl_Sens, vl_Voie, vl_Q, vl_IQ, vl_V, vl_IV, vl_TO, vl_ITO, vl_PL, vl_IPL;
	int vl_Nb_Param = 0;
	int vl_i;

	/* Variables locales pour les listes des stations */
	ERDT_DONNEES_RADT *pl_ListeRADT;

	/* Variables locales pour gestion des fichiers */
	XDY_Texte pl_PathEtFic = "";
	FILE *pl_Fd_Stock;
	int vl_index = 0;

	XZST_03EcritureTrace(XZSTC_WARNING, "IN : %s RADT = %d %s", version, va_Num_RADT, (va_Val_Suivante == XDC_VRAI) ? "suivant" : "renvoi");

	for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
		if (pl_ListeRADT->ConfRADT.NumeroStation == va_Num_RADT)
		{
			if (pl_ListeRADT->Etat_Service & XDC_EQT_HORS_SRV)
				return (XDC_OK);

			pl_Fd_Stock = pl_ListeRADT->Don_HisRADT.Fd;
			break;
		}

	/*B Construction du nom du fichier de stockage des mesures */
	sprintf(pl_PathEtFic, "%s/%s_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_STOCK_QVTO, va_Num_RADT);

	if (pl_Fd_Stock == NULL)
	{
		if (!access(pl_PathEtFic, F_OK))
		{
			errno = 0;
			if ((pl_Fd_Stock = fopen(pl_PathEtFic, "r+")) == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "OUT : %s Impossible d'ouvrir fic stock <%d> pour RADT = %d", version, errno, va_Num_RADT);
				return (XDC_NOK);
			}
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "OUT : %s Pas de fic stock pour RADT = %d", version, va_Num_RADT);
			return (XDC_OK);
		}
	}

	/*A Lecture du fichier jusqu'a plus de commentaire */
	while (fgets(pl_Ligne, sizeof(pl_Ligne), pl_Fd_Stock) != NULL)
		if (pl_Ligne[0] != ERDTC_COMMENTAIRE)
		{
			if (va_Val_Suivante == XDC_VRAI)
			{
				fseek(pl_Fd_Stock, -strlen(pl_Ligne), SEEK_CUR);
				pl_Ligne[0] = '#';
				XZST_03EcritureTrace(XZSTC_DEBUG1, "Memo mesure transmise pour RADT = %d <%s>", va_Num_RADT, pl_Ligne);
				fprintf(pl_Fd_Stock, "%s", pl_Ligne);
				fflush(pl_Fd_Stock);
				va_Val_Suivante = XDC_FAUX;
			}
			else
				break;
		}

	/*A Si fin du fichier => fermeture de celui-ci et effacement */
	if (feof(pl_Fd_Stock))
	{
		fclose(pl_Fd_Stock);
		XZSS_06DetruireFichier(pl_PathEtFic);
		if (!vg_mode_simu)
			pl_ListeRADT->Don_HisRADT.Fd = NULL;

		XZST_03EcritureTrace(XZSTC_WARNING, "OUT : %s Fin du fichier stock pour RADT = %d", version, va_Num_RADT);
		for (vl_i = 0; vl_i < ERDTC_NUM_MAX_STATION; vl_i++)
		{
			if (pg_Tab_RADT_Virtuelle[vl_i].NumStation1 == 0)
				continue;

			if ((pg_Tab_RADT_Virtuelle[vl_i].NumStation1 == va_Num_RADT) ||
				(pg_Tab_RADT_Virtuelle[vl_i].NumStation2 == va_Num_RADT) ||
				(pg_Tab_RADT_Virtuelle[vl_i].NumStation3 == va_Num_RADT))
			{
				er_Envoi_Mesure(pg_Tab_RADT_Virtuelle[vl_i].NumStationVirtuelle, va_Val_Suivante);
				XZST_03EcritureTrace(XZSTC_WARNING, "RADT = %d RADT virtuelle :%d", va_Num_RADT, pg_Tab_RADT_Virtuelle[vl_i].NumStationVirtuelle);
			}
		}
		return (XDC_OK);
	}
	if (!vg_mode_simu)
		pl_ListeRADT->Don_HisRADT.Fd = pl_Fd_Stock;
	XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : Mesure lue = %s", version, pl_Ligne);

	if ((vl_Nb_Param = sscanf(pl_Ligne, "%lf\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
							  &vl_Horodate, &vl_Sens, &vl_Voie, &vl_Q, &vl_IQ, &vl_V, &vl_IV, &vl_TO, &vl_ITO, &vl_PL, &vl_IPL)) != 11)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : Probleme Lecture ligne : %s ", version, pl_Ligne);
		return (XDC_NOK);
	}
	fseek(pl_Fd_Stock, -strlen(pl_Ligne), SEEK_CUR);

	/*B  Construction du DatGroup XDG_EMES_RADT_6mn<NomSite> */
	/*sprintf ( pl_DG, "%s_%s", XDG_EMES_RADT_6MN, vg_NomSite );*/

	/* SITEGESTION DEM748*/
	sprintf(pl_DG, "%s_%s", XDG_EMES_RADT_6MN, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion));
	XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Envoi %s", version, pl_DG);
	/*B Envoi du message XDM_EMES_6mn_Chaussee/XDM_EMES_6mn_Voie en fournissant les valeur recues */
	XZST_03EcritureTrace(XZSTC_INFO, "XDM_EMES_6mn_Chaussee/XDM_EMES_6mn_Voie %d %d %d", va_Num_RADT, vl_Sens, vl_Q);
	if (!TipcSrvMsgWrite(pl_DG, 
						 (vl_Voie == 0) ? XDM_IdentMsg(XDM_EMES_6mn_Chaussee) : XDM_IdentMsg(XDM_EMES_6mn_Voie), 
						 XDM_FLG_SRVMSGWRITE, 
						 T_IPC_FT_REAL8, vl_Horodate, 
						 T_IPC_FT_INT2, va_Num_RADT, 
						 T_IPC_FT_INT2, ERDTC_6MINUTE, 
						 T_IPC_FT_INT2, vl_Sens, 
						 T_IPC_FT_INT2, vl_Voie, 
						 T_IPC_FT_INT2, vl_Q, 
						 T_IPC_FT_INT2, vl_IQ, 
						 T_IPC_FT_INT2, vl_V, 
						 T_IPC_FT_INT2, vl_IV, 
						 T_IPC_FT_INT2, vl_TO, 
						 T_IPC_FT_INT2, vl_ITO, 
						 T_IPC_FT_INT2, vl_PL, 
						 T_IPC_FT_INT2, vl_IPL, 
						 NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : Envoi message XDM_EMES_6mn_Voie / Chaussee non effectue ", version);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_INFO, "%s : Envoi message XDM_EMES_6mn_Voie / Chaussee effectue avec succes", version);
	}
	/*B Envoi du message */
	TipcSrvFlush();

	if (pl_ListeRADT != NULL)
	{
		/*B  Construction du DatGroup XDG_EMES_RADT_V<NoEqt> pour les voisins */
		sprintf(pl_DG, "%s%d", XDG_EMES_RADT_V, va_Num_RADT);

		/*B Envoi du message XDM_EMES_RADT_6mn en fournissant les valeur recues */
		if (!TipcSrvMsgWrite(pl_DG, XDM_IdentMsg(XDM_EMES_RADT_6mn), XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_Horodate, T_IPC_FT_INT2, va_Num_RADT, T_IPC_FT_INT2, ERDTC_6MINUTE, T_IPC_FT_INT2, vl_Sens, T_IPC_FT_INT2, vl_Voie, T_IPC_FT_INT2, vl_Q, T_IPC_FT_INT2, vl_IQ, T_IPC_FT_INT2, vl_V, T_IPC_FT_INT2, vl_IV, T_IPC_FT_INT2, vl_TO, T_IPC_FT_INT2, vl_ITO, T_IPC_FT_INT2, vl_PL, T_IPC_FT_INT2, vl_IPL, NULL))
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "%s : Envoi message XDM_EMES_RADT_6mn non effectue ", version);
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_MESSAGE, "%s : Envoi message XDM_EMES_RADT_6mn effectue avec succes", version);
		}
		/*B Envoi du message */
		TipcSrvFlush();
	}

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi des mesures 6 minutes par voie et par chaussee (Lecture du m�moire)
*  pour les PC simplifi�s
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Env_Mes_6mn(XDY_Eqt va_Num_RADT, XDY_Entier va_Index)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT : Numero de la station RADT
*   va_Index    : Index du tableau de mesures
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
* Envoi des mesures 6 minutes par voie et par chaussee (Lecture m�moire)
* pour les PC simplifi�s
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Env_Mes_6mn";

	XDY_Datagroup pl_DG_gl_V;
	ERDT_DONNEES_Chaussee *pl_ListeChaussee;
	ERDT_DONNEES_PtMes *pl_ListePtMes;
	char pl_Ligne[250] = "";
	XDY_Horodate vl_Horodate;
	int vl_Numero, vl_Periode, vl_Chaussee, vl_Voie, vl_Q, vl_IQ, vl_V, vl_IV, vl_TO, vl_ITO, vl_PL, vl_IPL;
	int vl_Nb_Param = 0;
	ERDT_DONNEES_RADT *pl_ListeRADT;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s RADT = %d  Index = %d", version, va_Num_RADT, va_Index);

	for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
		if (pl_ListeRADT->ConfRADT.NumeroStation == va_Num_RADT)
		{
			break;
		}

	/*A Boucle sur le tableau de valeurs correspondant a la station RADT passee en argument
		en fonction des chaussees Sens1 et Sens 2 (uniquement) qui lui sont attribuees */
	for (vl_Chaussee = 1; vl_Chaussee < ERDTC_NB_CHAUSSEE; (vl_Chaussee == 4) ? vl_Chaussee += 3 : vl_Chaussee++)
	{
		/*A Envoi des donnees 6 mn Q, V, TO et PL pour les voies 0,1,2,3,4 */
		for (vl_Voie = 0; vl_Voie < ERDTC_NB_VOIE_CHAUS; vl_Voie++)
		{
			if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Voie != XDC_VOIE_INCONNUE)
			{
				/*B Construction du DatGroup XDG_EMES_RADT_6mn_R_<NoEqt><Chaussee><Voie> */
				sprintf(pl_DG_gl_V, "%s%d%d%d", XDG_EMES_RADT_6mn_R, va_Num_RADT, vl_Chaussee, vl_Voie);

				if (pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Numero == 197)
					XZST_03EcritureTrace(XZSTC_WARNING, "XDM_EMES_6mn_parVoie:vl_Voie=%d;Q=%d;V:%d;indicV=%d,PL=%d;TO=%d", vl_Voie, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Val, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val);

				/*B Envoi du message XDM_EMES_6mn_parVoie en fournissant les valeur recues */
				if (!TipcSrvMsgWrite(pl_DG_gl_V,
									 XDM_IdentMsg(XDM_EMES_6mn_parVoie),
									 XDM_FLG_SRVMSGWRITE,
									 T_IPC_FT_REAL8, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Numero,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Periode,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Sens,
									 T_IPC_FT_INT2, vl_Voie,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Val,
									 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Indic,
									 NULL))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EMES_6mn_parVoie non effectue");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EMES_6mn_parVoie effectue pour st :%d ch:%d voie:%d ", va_Num_RADT, vl_Chaussee, vl_Voie);
				}

				/*B Envoi du message */
				TipcSrvFlush();

				XZST_03EcritureTrace(XZSTC_DEBUG1, " DEM476 fonction er_Env_Mes_6mn %s : Horo = %lf\tStation = %d\tPeriode = %d\tSens = %d\tVoie = %d\tQ = %d\tQ = %d\tV = %d\tV = %d\tTO = %d\tTO = %d\tPL = %d\tPL = %d",
									 version,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Numero,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Periode,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Sens,
									 vl_Voie,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Val,
									 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Indic);

				if (vg_Mac_Ext_Ouest)
				{
					if (vl_Voie == 0)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, " DEM476 fonction er_Env_Mes_6mn suite%s : Horo = %lf\tStation = %d\tPeriode = %d\tSens = %d\tVoie = %d\tQ = %d\tQ = %d\tV = %d\tV = %d\tTO = %d\tTO = %d\tPL = %d\tPL = %d",
											 version,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Numero,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Periode,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Sens,
											 vl_Voie,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Val,
											 pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Indic);
						/*B Envoi de XDM_EMES_6gl_Chaussee dans XDG_EMES_RADT_6GL_R_<NoEqt> */

						/*B Construction du DatGroup XDG_EMES_RADT_6GL_R_<NoEqt> */
						/*    sprintf ( pl_DG_gl_V, "%s_%s", XDG_EMES_RADT_6GL_R, vg_NomSite ); DEM748*/
						sprintf(pl_DG_gl_V, "%s_%s", XDG_EMES_RADT_6GL_R, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion));

						XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Envoi du message XDM_EMES_6gl_Chaussee sur <%s>", version, pl_DG_gl_V);

						/*B Envoi du message XDM_EMES_6gl_Chaussee en fournissant les valeur recues */
						if (!TipcSrvMsgWrite(pl_DG_gl_V,
											 XDM_IdentMsg(XDM_EMES_6gl_Chaussee),
											 XDM_FLG_SRVMSGWRITE,
											 T_IPC_FT_REAL8, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate,
											 T_IPC_FT_STR, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion),
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Numero,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Periode,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Sens,
											 T_IPC_FT_INT2, XDC_RADT_CHAUSSEE,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Val,
											 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Indic,
											 NULL))
						{
							XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EInit_IHM_6gl_Chaussee non effectue");
						}
						else
						{
							XZST_03EcritureTrace(XZSTC_MESSAGE, " Envoi message XDM_EInit_IHM_6gl_Chaussee effectue");
							TipcSrvMsgWrite(XDG_EMES_RADT_6GL_R,
											XDM_IdentMsg(XDM_EMES_6gl_Chaussee),
											XDM_FLG_SRVMSGWRITE,
											T_IPC_FT_REAL8, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Horodate,
											T_IPC_FT_STR, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion),
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Numero,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Periode,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Sens,
											T_IPC_FT_INT2, XDC_RADT_CHAUSSEE,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Val,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValQ.Indic,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Val,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValV.Indic,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Val,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][0][va_Index].Mesures.ValTO.Indic,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Val,
											T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][1][va_Index].Mesures.ValPL.Indic,
											NULL);
						}

						/*B Envoi du message */
						TipcSrvFlush();
					}

					/*B Construction du DatGroup XDG_EMES_RADT_6GL_R_<NoEqt> */
					sprintf(pl_DG_gl_V, "%s%d%d%d", XDG_EMES_RADT_6GL_R, va_Num_RADT, vl_Chaussee, vl_Voie);
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : Envoi du message XDM_EMES_6gl_Voie<NoEqt> sur <%s>", version, pl_DG_gl_V);

					/*B Envoi du message XDM_EMES_6gl_Voie en fournissant les valeur recues */
					if (!TipcSrvMsgWrite(pl_DG_gl_V,
										 XDM_IdentMsg(XDM_EMES_6gl_Voie),
										 XDM_FLG_SRVMSGWRITE,
										 T_IPC_FT_REAL8, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Horodate,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Numero,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Periode,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Sens,
										 T_IPC_FT_INT2, vl_Voie,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Val,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValQ.Indic,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Val,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValV.Indic,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Val,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][vl_Voie][va_Index].Mesures.ValTO.Indic,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Val,
										 T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[va_Num_RADT]][vl_Chaussee][(vl_Voie == 0) ? 1 : vl_Voie][va_Index].Mesures.ValPL.Indic,
										 NULL))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EInit_IHM_6gl_Voie non effectue");
					}
					else
					{
						XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EInit_IHM_6gl_Voie effectue");
					}
				}
			}
		}
	}

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Stockage des mesures pesage dans un fichier
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Stockage_Pesage(ERDT_MESURE_PESAGE va_Pesage, XDY_Entier va_Mesures_Valides)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT : Numero de la station RADT
*
* ARGUMENTS EN SORTIE :
*
*   pa_Mesure   : Mesure Chaussee
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*  Stockage des mesures pesage dans un fichier
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Stockage_Pesage";

	XDY_Mot vl_Chaussee = 0;
	XDY_Mot vl_Voie = 0;
	XDY_Mot vl_Index = 0;
	XDY_Mot vl_Cpt = 0;
	FILE *vl_Fd;
	XDY_Texte pl_PathEtFic = "";
	XDY_Texte pl_PathEtFicLect = "";
	char pl_Ligne[250] = "";

	XZST_03EcritureTrace(XZSTC_WARNING, "IN %s : RADT = %d  Type = %d", version, va_Pesage.NumeroStation, va_Pesage.Type);
	/*A Construction du nom du fichier */
	sprintf(pl_PathEtFic, "%s/%s_%d_W%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_STOCK_PESAGE, va_Pesage.Type, va_Pesage.NumeroStation);
	vl_Fd = fopen(pl_PathEtFic, "a+");
	if (vl_Fd == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s Impossible de creer le fichier <%s>", version, pl_PathEtFic);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne NOK", version);
		return (XDC_NOK);
	}

	for (vl_Chaussee = 1; vl_Chaussee < ERDTC_NB_CHAUSSEE; (vl_Chaussee == 4) ? vl_Chaussee += 3 : vl_Chaussee++)
		for (vl_Voie = (ERDTC_NB_VOIE_CHAUS - 1); vl_Voie > -1; vl_Voie--)
		{
			if (va_Pesage.Mesure[vl_Chaussee][vl_Voie].Voie != XDC_VOIE_INCONNUE)
			{
				vl_Cpt = sprintf(pl_Ligne, "%.0lf\t%d\t%d\t", va_Pesage.Horodate, vl_Chaussee, vl_Voie);
				for (vl_Index = 0; vl_Index < va_Pesage.NbClasse; vl_Index++)
					vl_Cpt += sprintf(&pl_Ligne[vl_Cpt], "%d\t", va_Pesage.Mesure[vl_Chaussee][vl_Voie].Classe[vl_Index]);
				fprintf(vl_Fd, "%s\n", pl_Ligne);
			}
		}

	fclose(vl_Fd);
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne 0", version);

	/*A Recopie du fichier ecriture si pas de fichier lecture */
	sprintf(pl_PathEtFicLect, "%s/%s_%d_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_STOCK_PESAGE, va_Pesage.Type, va_Pesage.NumeroStation);
	if (access(pl_PathEtFicLect, F_OK))
		rename(pl_PathEtFic, pl_PathEtFicLect);

	/*B Ecriture de l'horodate des dernieres mesures pesage recues dans un fichier si mesures valides */
	if (va_Mesures_Valides)
		er_Maj_Fichier_heure_Pesage(va_Pesage.NumeroStation);

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Mise a jour du fichier derniere heure d'acquisition des pesages
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Maj_Fichier_heure_Pesage(XDY_Eqt va_Num_RADT)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT 	: Numero de la station RADT
*   va_TypeMesure	: type de mesure ( silhouette, essieu, poids )
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Maj_Fichier_heure_Pesage ";
	FILE *vl_Fd;
	XDY_Texte pl_PathEtFic = "";
	XZSMT_Horodate vl_heure_string_his = "";
	ERDT_DONNEES_RADT *pl_ListeRADT;

	/*A Stockage de la derniere heure d'acquisition du pesage*/
	XZST_03EcritureTrace(XZSTC_INFO, "%s : MAJ HR Pesage St %d ", version, va_Num_RADT);
	sprintf(pl_PathEtFic, "%s/%s_%s_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_HEUR_PESAGE, vg_NomMachine, va_Num_RADT);
	if ((vl_Fd = fopen(pl_PathEtFic, "w+")) == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : fichier %s. introuvable", version, pl_PathEtFic);
		return (XDC_NOK);
	}
	else
	{
		for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
			if (pl_ListeRADT->ConfRADT.NumeroStation == va_Num_RADT)
			{
				XZSM_11ConversionHorodate(pl_ListeRADT->Don_HisSilhouette.Horodate_Histo, &vl_heure_string_his);
				fprintf(vl_Fd, "%s \n", vl_heure_string_his);
				XZSM_11ConversionHorodate(pl_ListeRADT->Don_HisEssieu.Horodate_Histo, &vl_heure_string_his);
				fprintf(vl_Fd, "%s \n", vl_heure_string_his);
				XZSM_11ConversionHorodate(pl_ListeRADT->Don_HisPoids.Horodate_Histo, &vl_heure_string_his);
				fprintf(vl_Fd, "%s \n", vl_heure_string_his);
				break;
			}

		fclose(vl_Fd);
	}

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi des mesures de pesage (Lecture du fichier)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Envoi_Mesure_Pesage(XDY_Eqt va_Num_RADT, XDY_Booleen va_Val_Suivante, XDY_Entier va_TypeMesure)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT     : Numero de la station RADT
*   va_Val_Suivante : Booleen pour designer s'il faut lire la valeur suivante ou renvoyer la derniere
*   va_TypeMesure   : type de mesure ( silhouette, essieu, poids )
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
* Envoi des mesures de pesage (Lecture du fichier)
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Envoi_Mesure_Pesage";

	XDY_Datagroup pl_DG = "";
	char pl_Ligne[250] = "";
	int vl_Sens = 0;
	int vl_NbClasse = 0;
	int vl_Voie = 0;
	XDY_Horodate vl_Horodate = 0;
	int vl_Nb_Param = 0;
	int vl_Index = 0;
	char *pl_Texte;
	int vl_Pesage[ERDTC_NB_MAX_CLAS];
	/* Variables locales pour les listes des stations */
	ERDT_DONNEES_RADT *pl_ListeRADT;

	/* Variables locales pour gestion des fichiers */

	XDY_Texte pl_PathEtFic = "";
	FILE *pl_Fd_Stock;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s RADT = %d %s", version, va_Num_RADT, (va_Val_Suivante == XDC_VRAI) ? "suivant" : "renvoi");

	for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
		if (pl_ListeRADT->ConfRADT.NumeroStation == va_Num_RADT)
		{
			switch (va_TypeMesure)
			{
			case XDC_MESURE_SILHOUETTE:
				pl_Fd_Stock = pl_ListeRADT->Don_HisSilhouette.Fd;
				break;
			case XDC_MESURE_ESSIEU:
				pl_Fd_Stock = pl_ListeRADT->Don_HisEssieu.Fd;
				break;
			case XDC_MESURE_POIDS:
				pl_Fd_Stock = pl_ListeRADT->Don_HisPoids.Fd;
				break;
			}
			break;
		}

	/*B Construction du nom du fichier de stockage des mesures */
	sprintf(pl_PathEtFic, "%s/%s_%d_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_STOCK_PESAGE, va_TypeMesure, va_Num_RADT);

	if (pl_Fd_Stock == NULL)
	{
		if (!access(pl_PathEtFic, F_OK))
		{
			errno = 0;
			if ((pl_Fd_Stock = fopen(pl_PathEtFic, "r+")) == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "OUT : %s Impossible d'ouvrir fic stock(%d) <%d> pour RADT = %d", version, va_TypeMesure, errno, va_Num_RADT);
				return (XDC_NOK);
			}
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "OUT : %s Pas de fic stock(%d) pour RADT = %d", version, va_TypeMesure, va_Num_RADT);
			return (XDC_OK);
		}
	}

	/*A Lecture du fichier jusqu'a plus de commentaire */
	while (fgets(pl_Ligne, sizeof(pl_Ligne), pl_Fd_Stock) != NULL)
		if (pl_Ligne[0] != ERDTC_COMMENTAIRE)
		{
			if (va_Val_Suivante == XDC_VRAI)
			{
				fseek(pl_Fd_Stock, -strlen(pl_Ligne), SEEK_CUR);
				pl_Ligne[0] = '#';
				XZST_03EcritureTrace(XZSTC_DEBUG1, "Memo mesure(%d) transmise pour RADT = %d <%s>", va_TypeMesure, va_Num_RADT, pl_Ligne);
				fprintf(pl_Fd_Stock, "%s", pl_Ligne);
				fflush(pl_Fd_Stock);
				va_Val_Suivante = XDC_FAUX;
			}
			else
				break;
		}

	/*A Si fin du fichier => fermeture de celui-ci et effacement */
	if (feof(pl_Fd_Stock))
	{
		fclose(pl_Fd_Stock);
		XZSS_06DetruireFichier(pl_PathEtFic);
		pl_Fd_Stock = NULL;
		XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT : %s Fin du fichier stock(%d) pour RADT = %d", version, va_TypeMesure, va_Num_RADT);
	}

	switch (va_TypeMesure)
	{
	case XDC_MESURE_SILHOUETTE:
		pl_ListeRADT->Don_HisSilhouette.Fd = pl_Fd_Stock;
		vl_NbClasse = XDC_NB_CLASSE_SILHOUETTE;
		break;
	case XDC_MESURE_ESSIEU:
		pl_ListeRADT->Don_HisEssieu.Fd = pl_Fd_Stock;
		vl_NbClasse = XDC_NB_CLASSE_ESSIEU;
		break;
	case XDC_MESURE_POIDS:
		pl_ListeRADT->Don_HisPoids.Fd = pl_Fd_Stock;
		vl_NbClasse = XDC_NB_CLASSE_POIDS;
		break;
	}

	if (pl_Fd_Stock == NULL)
		return (XDC_OK);

	XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : Mesure lue(%d) = %s", version, va_TypeMesure, pl_Ligne);

	if ((vl_Nb_Param = sscanf(pl_Ligne, "%lf\t%d\t%d\t",
							  &vl_Horodate, &vl_Sens, &vl_Voie)) != 3)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : Probleme Lecture ligne : %s ", version, pl_Ligne);
		return (XDC_NOK);
	}
	fseek(pl_Fd_Stock, -strlen(pl_Ligne), SEEK_CUR);

	pl_Texte = strtok(pl_Ligne, "\t");
	pl_Texte = strtok(NULL, "\t");
	pl_Texte = strtok(NULL, "\t");

	while (((pl_Texte = strtok(NULL, "\t")) != NULL) && (vl_Index < vl_NbClasse))
		if ((vl_Nb_Param = sscanf(pl_Texte, "%d", &vl_Pesage[vl_Index++])) != 1)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "%s : Probleme Lecture ligne : %s ", version, pl_Ligne);
			return (XDC_NOK);
		}

	/*B  Construction du DatGroup XDG_EMES_PESAGE<NomSite> */
	/*sprintf ( pl_DG, "%s_%s", XDG_EMES_PESAGE, vg_NomSite ); DEM748*/
	sprintf(pl_DG, "%s_%s", XDG_EMES_PESAGE, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion));
	XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Envoi %s", version, pl_DG);

	/*B Envoi du message XDM_EMES_PESAGE en fournissant les valeurs lues */
	if (!TipcSrvMsgWrite(pl_DG, XDM_IdentMsg(XDM_EMES_PESAGE), XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_Horodate, T_IPC_FT_INT2, va_Num_RADT, T_IPC_FT_INT2, vl_Sens, T_IPC_FT_INT2, vl_Voie, T_IPC_FT_INT2, va_TypeMesure, T_IPC_FT_INT4_ARRAY, vl_Pesage, vl_NbClasse, NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : Envoi message XDM_EMES_PESAGE non effectue ", version);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s : Envoi message XDM_EMES_PESAGE effectue avec succes", version);
	}
	/*B Envoi du message */
	TipcSrvFlush();

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Recopie du fichier des mesures de pesage
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Recopie_Fichier_Mesure_Pesage(XDY_Eqt va_Num_RADT, XDY_Entier va_TypeMesure)

/*
* ARGUMENTS EN ENTREE :
*
*   va_Num_RADT     : Numero de la station RADT
*   va_TypeMesure   : type de mesure ( silhouette, essieu, poids )
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*  Recopie du fichier des mesures de pesage
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ :  er_Recopie_Fichier_Mesure_Pesage ";

	/* Variables locales pour gestion des fichiers */

	XDY_Texte pl_PathEtFic = "";
	XDY_Texte pl_PathEtFicLect = "";

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s RADT = %d %s", version, va_Num_RADT);

	/*B Construction du nom du fichier de stockage des mesures */
	sprintf(pl_PathEtFic, "%s/%s_%d_W%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_STOCK_PESAGE, va_TypeMesure, va_Num_RADT);
	sprintf(pl_PathEtFicLect, "%s/%s_%d_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_STOCK_PESAGE, va_TypeMesure, va_Num_RADT);
	/*B si pas de fichier lecture et existance d'un fichier ecriture */
	if (access(pl_PathEtFicLect, F_OK) && !access(pl_PathEtFic, F_OK))
		/*B Alors recopie du fichier et envoie des mesures (appel er_Envoi_Mesure_Pesage) */
		rename(pl_PathEtFic, pl_PathEtFicLect);

	if (!access(pl_PathEtFicLect, F_OK) && (vg_Etat_Liaison_CI == ERDTC_LIAISON_CI_OK))
		er_Envoi_Mesure_Pesage(va_Num_RADT, XDC_FAUX, va_TypeMesure);

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Calcul des donnees glissantes 6mn
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_Calcul_RADT_Virtuelle(XDY_Eqt va_Num_RADT,
							 XDY_Entier va_Freq_Mes,
							 XDY_Entier va_Index)

/*
* ARGUMENTS EN ENTREE :
*   va_Num_RADT : Numero Station RADT
*   va_Index	: Index du tableau des valeurs
*   va_Freq_Mes	: Frequence ( 1 ou 6 minutes )
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
* Calcul des donnees glissantes 6mn :
* 	Calcul des donnees glissantes par chaussee
*	Envoi de XDM_EMES_6gl_Chaussee dans XDG_EMES_RADT_6mn_R<NoEqt>
*
------------------------------------------------------*/
{
	static char *version = "$Id: erdt_cal.c,v 1.67 2021/01/29 14:49:11 akkaakka Exp $ : er_Calcul_RADT_Virtuelle ";

	ERDT_EMes_RADT *pl_Mes_Chaussee1 = NULL;
	ERDT_EMes_RADT *pl_Mes_Chaussee2 = NULL;
	ERDT_EMes_RADT *pl_Mes_Chaussee3 = NULL;
	ERDT_EMes_RADT vl_Mes_Virtuelle;
	ERDT_DONNEES_RADT *pl_ListeRADT;
	XDY_Datagroup pl_DG_gl_Ch;
	XDY_Sens vl_Chaussee1 = 0;
	XDY_Sens vl_Chaussee1Neg = 1;
	XDY_Sens vl_Chaussee2 = 0;
	XDY_Sens vl_Chaussee2Neg = 1;
	XDY_Sens vl_Chaussee3 = 0;
	XDY_Sens vl_Chaussee3Neg = 1;
	XDY_Eqt vl_Num_RADT = 0;
	XDY_Eqt vl_Num_RADT1 = 0;
	XDY_Eqt vl_Num_RADT2 = 0;
	XDY_Eqt vl_Num_RADT3 = 0;
	XDY_Entier vl_Index = 0;
	XDY_Entier vl_Index2 = 0;
	XDY_Entier vl_Index3 = 0;
	XDY_Entier vl_Cpt = 0;
	XDY_Coeff ValV = 0.0;

	/*A Boucle sur le tableau des stations RADT Virtuelles passee en argument
		 qui lui sont attribuees */
	for (vl_Index = 0; vl_Index < ERDTC_NUM_MAX_STATION; vl_Index++)
	{
		if ((pg_Tab_RADT_Virtuelle[vl_Index].NumStation1 == va_Num_RADT) ||
			(pg_Tab_RADT_Virtuelle[vl_Index].NumStation2 == va_Num_RADT) ||
			(pg_Tab_RADT_Virtuelle[vl_Index].NumStation3 == va_Num_RADT))
		{

			for (pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant)
				if (pl_ListeRADT->ConfRADT.NumeroStation == pg_Tab_RADT_Virtuelle[vl_Index].NumStationVirtuelle)
					if (pl_ListeRADT->Etat_Service & XDC_EQT_HORS_SRV)
						return (XDC_OK);
					else
						break;

			if (pg_Tab_RADT_Virtuelle[vl_Index].Chaussee1 < 0)
				vl_Chaussee1Neg = -1;
			if (pg_Tab_RADT_Virtuelle[vl_Index].Chaussee2 < 0)
				vl_Chaussee2Neg = -1;
			if (pg_Tab_RADT_Virtuelle[vl_Index].Chaussee3 < 0)
				vl_Chaussee3Neg = -1;
			vl_Chaussee1 = abs(pg_Tab_RADT_Virtuelle[vl_Index].Chaussee1);
			vl_Chaussee2 = abs(pg_Tab_RADT_Virtuelle[vl_Index].Chaussee2);
			vl_Chaussee3 = abs(pg_Tab_RADT_Virtuelle[vl_Index].Chaussee3);
			vl_Num_RADT = pg_Tab_RADT[va_Num_RADT];
			vl_Num_RADT1 = pg_Tab_RADT_Virtuelle[vl_Index].NumStation1;
			vl_Num_RADT2 = pg_Tab_RADT_Virtuelle[vl_Index].NumStation2;
			vl_Num_RADT3 = pg_Tab_RADT_Virtuelle[vl_Index].NumStation3;
			if (vl_Num_RADT2 > 0)
				vl_Num_RADT2 = ((er_mesures_valides(vl_Num_RADT2, vl_Chaussee2)) >= 0) ? vl_Num_RADT2 : 0;
			if (vl_Num_RADT3 > 0)
				vl_Num_RADT3 = ((er_mesures_valides(vl_Num_RADT3, vl_Chaussee3)) >= 0) ? vl_Num_RADT3 : 0;

			XZST_03EcritureTrace(XZSTC_WARNING, "%d %d %d", vl_Num_RADT1, vl_Num_RADT2, vl_Num_RADT3);
			switch (va_Freq_Mes)
			{
			case ERDTC_MINUTE:
				pl_Mes_Chaussee1 = &(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT1]][vl_Chaussee1][0][va_Index]);
				pl_Mes_Chaussee1->Mesures.ValPL.Indic = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT1]][vl_Chaussee1][1][va_Index].Mesures.ValPL.Indic;
				pl_Mes_Chaussee1->Mesures.ValPL.Val = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT1]][vl_Chaussee1][1][va_Index].Mesures.ValPL.Val;
				pl_Mes_Chaussee2 = pl_Mes_Chaussee1;
				pl_Mes_Chaussee3 = pl_Mes_Chaussee1;
				if (vl_Num_RADT2 > 0)
				{
					pl_Mes_Chaussee2 = &(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT2]][vl_Chaussee2][0][va_Index]);
					pl_Mes_Chaussee2->Mesures.ValPL.Indic = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT2]][vl_Chaussee2][1][va_Index].Mesures.ValPL.Indic;
					pl_Mes_Chaussee2->Mesures.ValPL.Val = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT2]][vl_Chaussee2][1][va_Index].Mesures.ValPL.Val;
				}
				if (vl_Num_RADT3 > 0)
				{
					pl_Mes_Chaussee3 = &(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT3]][vl_Chaussee3][0][va_Index]);
					pl_Mes_Chaussee3->Mesures.ValPL.Indic = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT3]][vl_Chaussee3][1][va_Index].Mesures.ValPL.Indic;
					pl_Mes_Chaussee3->Mesures.ValPL.Val = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Num_RADT3]][vl_Chaussee3][1][va_Index].Mesures.ValPL.Val;
				}
				break;

			case ERDTC_6MINUTE:
				pl_Mes_Chaussee1 = &(pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT1]][vl_Chaussee1][0][va_Index]);
				pl_Mes_Chaussee2 = pl_Mes_Chaussee1;
				pl_Mes_Chaussee3 = pl_Mes_Chaussee1;
				if (vl_Num_RADT2 > 0)
				{
					pl_Mes_Chaussee2 = &(pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT2]][vl_Chaussee2][0][va_Index]);
					pl_Mes_Chaussee2->Mesures.ValPL.Indic = pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT2]][vl_Chaussee2][1][va_Index].Mesures.ValPL.Indic;
					pl_Mes_Chaussee2->Mesures.ValPL.Val = pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT2]][vl_Chaussee2][1][va_Index].Mesures.ValPL.Val;
				}
				if (vl_Num_RADT3 > 0)
				{
					pl_Mes_Chaussee3 = &(pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT3]][vl_Chaussee3][0][va_Index]);
					pl_Mes_Chaussee3->Mesures.ValPL.Indic = pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT3]][vl_Chaussee3][1][va_Index].Mesures.ValPL.Indic;
					pl_Mes_Chaussee3->Mesures.ValPL.Val = pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Num_RADT3]][vl_Chaussee3][1][va_Index].Mesures.ValPL.Val;
				}
				break;
			}
			memcpy(&vl_Mes_Virtuelle, pl_Mes_Chaussee1, sizeof(ERDT_EMes_RADT));
			vl_Mes_Virtuelle.Sens = pg_Tab_RADT_Virtuelle[vl_Index].Sens;
			vl_Mes_Virtuelle.Numero = pg_Tab_RADT_Virtuelle[vl_Index].NumStationVirtuelle;

			/* Calcul de la chaussee virtuelle */
			if ((vl_Num_RADT2 > 0) || (vl_Num_RADT3 > 0))
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "pl_Mes_Chaussee1->Voie %d, pl_Mes_Chaussee2->Voie %d, pl_Mes_Chaussee3->Voie %d",
									 pl_Mes_Chaussee1->Voie, pl_Mes_Chaussee2->Voie, pl_Mes_Chaussee3->Voie);
				XZST_03EcritureTrace(XZSTC_WARNING, "pl_Mes_Chaussee1->debit %d", pl_Mes_Chaussee1->Mesures.ValQ.Val);
				if ((pl_Mes_Chaussee1->Voie != XDC_VOIE_INCONNUE) && ((vl_Num_RADT2 < 0) || (pl_Mes_Chaussee2->Voie != XDC_VOIE_INCONNUE)) &&
					((vl_Num_RADT3 < 0) || (pl_Mes_Chaussee3->Voie != XDC_VOIE_INCONNUE)))
				{
					if ((pl_Mes_Chaussee1->Mesures.ValQ.Indic != XDC_RADT_INCONNU) &&
						((vl_Num_RADT2 < 0) || (pl_Mes_Chaussee2->Mesures.ValQ.Indic != XDC_RADT_INCONNU)) &&
						((vl_Num_RADT3 < 0) || (pl_Mes_Chaussee3->Mesures.ValQ.Indic != XDC_RADT_INCONNU)))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "ValQ %d %d %d ",
											 vl_Mes_Virtuelle.Mesures.ValQ.Val,
											 pl_Mes_Chaussee2->Mesures.ValQ.Val,
											 pl_Mes_Chaussee3->Mesures.ValQ.Val);
						if (vl_Num_RADT2 > 0)
							vl_Mes_Virtuelle.Mesures.ValQ.Val += pl_Mes_Chaussee2->Mesures.ValQ.Val * vl_Chaussee2Neg;
						if (vl_Num_RADT3 > 0)
							vl_Mes_Virtuelle.Mesures.ValQ.Val += pl_Mes_Chaussee3->Mesures.ValQ.Val * vl_Chaussee3Neg;
						XZST_03EcritureTrace(XZSTC_WARNING, "ValQ virutelle %d", vl_Mes_Virtuelle.Mesures.ValQ.Val);
					}

					if ((pl_Mes_Chaussee1->Mesures.ValV.Indic != XDC_RADT_INCONNU) &&
						((vl_Num_RADT2 < 0) || (pl_Mes_Chaussee2->Mesures.ValV.Indic != XDC_RADT_INCONNU)) &&
						((vl_Num_RADT3 < 0) || (pl_Mes_Chaussee3->Mesures.ValV.Indic != XDC_RADT_INCONNU)))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "ValV %d %d %d",
											 pl_Mes_Chaussee1->Mesures.ValV.Val,
											 pl_Mes_Chaussee2->Mesures.ValV.Val,
											 pl_Mes_Chaussee3->Mesures.ValV.Val);
						ValV = (float)(pl_Mes_Chaussee1->Mesures.ValQ.Val / pl_Mes_Chaussee1->Mesures.ValV.Val);
						if (vl_Num_RADT2 > 0)
							ValV += (float)(pl_Mes_Chaussee2->Mesures.ValQ.Val / pl_Mes_Chaussee2->Mesures.ValV.Val);
						if (vl_Num_RADT3 > 0)
							ValV += (float)(pl_Mes_Chaussee3->Mesures.ValQ.Val / pl_Mes_Chaussee3->Mesures.ValV.Val);
						vl_Mes_Virtuelle.Mesures.ValV.Val = (float)(vl_Mes_Virtuelle.Mesures.ValQ.Val / ValV);
						XZST_03EcritureTrace(XZSTC_WARNING, "ValV virtuelle %d", vl_Mes_Virtuelle.Mesures.ValV.Val);
					}

					if ((pl_Mes_Chaussee1->Mesures.ValTO.Indic != XDC_RADT_INCONNU) &&
						((vl_Num_RADT2 < 0) || (pl_Mes_Chaussee2->Mesures.ValTO.Indic != XDC_RADT_INCONNU)) &&
						((vl_Num_RADT3 < 0) || (pl_Mes_Chaussee3->Mesures.ValTO.Indic != XDC_RADT_INCONNU)))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "ValTO %d %d %d ",
											 pl_Mes_Chaussee1->Mesures.ValTO.Val,
											 pl_Mes_Chaussee2->Mesures.ValTO.Val,
											 pl_Mes_Chaussee3->Mesures.ValTO.Val);
						vl_Mes_Virtuelle.Mesures.ValTO.Val = (float)pl_Mes_Chaussee1->Mesures.ValTO.Val;
						if ((vl_Num_RADT2 > 0) && (vl_Num_RADT3 > 0))
							vl_Mes_Virtuelle.Mesures.ValTO.Val = (float)((pl_Mes_Chaussee1->Mesures.ValTO.Val + pl_Mes_Chaussee2->Mesures.ValTO.Val + pl_Mes_Chaussee3->Mesures.ValTO.Val) / 3);
						else if ((vl_Num_RADT2 > 0) && (vl_Num_RADT3 < 0))
							vl_Mes_Virtuelle.Mesures.ValTO.Val = (float)((pl_Mes_Chaussee1->Mesures.ValTO.Val + pl_Mes_Chaussee2->Mesures.ValTO.Val) / 2);
						else if (vl_Num_RADT3 > 0)
							vl_Mes_Virtuelle.Mesures.ValTO.Val = (float)((pl_Mes_Chaussee1->Mesures.ValTO.Val + pl_Mes_Chaussee3->Mesures.ValTO.Val) / 2);
						XZST_03EcritureTrace(XZSTC_WARNING, "ValTO virtuelle %d", vl_Mes_Virtuelle.Mesures.ValTO.Val);
					}

					if ((pl_Mes_Chaussee1->Mesures.ValPL.Val != XDC_RADT_PL_INCONNU) &&
						((vl_Num_RADT2 < 0) || (pl_Mes_Chaussee2->Mesures.ValPL.Val != XDC_RADT_PL_INCONNU)) &&
						((vl_Num_RADT3 < 0) || (pl_Mes_Chaussee3->Mesures.ValPL.Val != XDC_RADT_PL_INCONNU)) &&
						((vl_Num_RADT2 < 0) || (pl_Mes_Chaussee2->Mesures.ValPL.Val != 0)) &&
						((vl_Num_RADT3 < 0) || (pl_Mes_Chaussee3->Mesures.ValPL.Val != 0)))
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "ValPL");
						if ((vl_Num_RADT2 > 0) && (vl_Num_RADT3 > 0))
							vl_Mes_Virtuelle.Mesures.ValPL.Val = (float)((pl_Mes_Chaussee1->Mesures.ValPL.Val +
																		  pl_Mes_Chaussee2->Mesures.ValPL.Val + pl_Mes_Chaussee3->Mesures.ValPL.Val) /
																		 3);
						else if ((vl_Num_RADT2 > 0) && (vl_Num_RADT3 < 0))
							vl_Mes_Virtuelle.Mesures.ValPL.Val = (float)((pl_Mes_Chaussee1->Mesures.ValPL.Val +
																		  pl_Mes_Chaussee2->Mesures.ValPL.Val) /
																		 2);
						else if (vl_Num_RADT3 > 0)
							vl_Mes_Virtuelle.Mesures.ValPL.Val = (float)((pl_Mes_Chaussee1->Mesures.ValPL.Val +
																		  pl_Mes_Chaussee3->Mesures.ValPL.Val) /
																		 2);
					}
				}
			}

			XZST_03EcritureTrace(XZSTC_WARNING, "va_Freq_Mes %d", va_Freq_Mes);
			if (va_Freq_Mes == ERDTC_MINUTE)
			{
				/*B Stockage des mesures par voie et par chaussee */
				memcpy(&(pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_Mes_Virtuelle.Numero]][vl_Mes_Virtuelle.Sens][0][va_Index]),
					   &vl_Mes_Virtuelle,
					   sizeof(ERDT_EMes_RADT));
				/*B Construction du DatGroup XDG_EMES_RADT_6GL_R_<NomSite> */
				/*sprintf ( pl_DG_gl_Ch, "%s_%s", XDG_EMES_RADT_6GL_R, vg_NomSite );*/

				/* SITEGESTION DEM748*/
				sprintf(pl_DG_gl_Ch, "%s_%s", XDG_EMES_RADT_6GL_R, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion));
				XZST_03EcritureTrace(XZSTC_DEBUG3, "%s : Envoi %s", version, pl_DG_gl_Ch);

				/*B Envoi du message XDM_EMES_6gl_Chaussee en fournissant les valeur recues */
				if (!TipcSrvMsgWrite(pl_DG_gl_Ch,
									 XDM_IdentMsg(XDM_EMES_6gl_Chaussee),
									 XDM_FLG_SRVMSGWRITE,
									 T_IPC_FT_REAL8, vl_Mes_Virtuelle.Horodate,
									 T_IPC_FT_STR, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion),
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Numero,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Periode,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Sens,
									 T_IPC_FT_INT2, XDC_RADT_CHAUSSEE,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValQ.Val,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValQ.Indic,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValV.Val,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValV.Indic,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValTO.Val,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValTO.Indic,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValPL.Val,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValPL.Indic,
									 NULL))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EInit_IHM_6gl_Chaussee non effectue");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EInit_IHM_6gl_Chaussee effectue");
					if (va_Index == 0)
						TipcSrvMsgWrite(XDG_EMES_RADT_6GL_R,
										XDM_IdentMsg(XDM_EMES_6gl_Chaussee),
										XDM_FLG_SRVMSGWRITE,
										T_IPC_FT_REAL8, vl_Mes_Virtuelle.Horodate,
										T_IPC_FT_STR, XZSC23_Nom_From_Numero(pl_ListeRADT->ConfRADT.SiteGestion),
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Numero,
										T_IPC_FT_INT2, ERDTC_6MINUTE,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Sens,
										T_IPC_FT_INT2, XDC_RADT_CHAUSSEE,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValQ.Val,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValQ.Indic,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValV.Val,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValV.Indic,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValTO.Val,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValTO.Indic,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValPL.Val,
										T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValPL.Indic,
										NULL);
				}

				/*B Envoi du message */
				TipcSrvFlush();

				XZST_03EcritureTrace(XZSTC_WARNING, "%s : Horo = %lf\tStation = %d\tPeriode = %d\tSens = %d\tQ = %d\tQ = %d\tV = %d\tV = %d\tTO = %d\tTO = %d\tPL = %d\tPL = %d\n",
									 version,
									 vl_Mes_Virtuelle.Horodate,
									 vl_Mes_Virtuelle.Numero,
									 vl_Mes_Virtuelle.Periode,
									 vl_Mes_Virtuelle.Sens,
									 vl_Mes_Virtuelle.Mesures.ValQ.Val,
									 vl_Mes_Virtuelle.Mesures.ValQ.Indic,
									 vl_Mes_Virtuelle.Mesures.ValV.Val,
									 vl_Mes_Virtuelle.Mesures.ValV.Indic,
									 vl_Mes_Virtuelle.Mesures.ValTO.Val,
									 vl_Mes_Virtuelle.Mesures.ValTO.Indic,
									 vl_Mes_Virtuelle.Mesures.ValPL.Val,
									 vl_Mes_Virtuelle.Mesures.ValPL.Indic);

				sprintf(pl_DG_gl_Ch, "%s%d%d%d", XDG_EMES_RADT_6GL_R, va_Num_RADT, vl_Mes_Virtuelle.Sens, XDC_RADT_CHAUSSEE);
				/*B Envoi du message XDM_EMES_6gl_Voie en fournissant les valeur recues */
				if (!TipcSrvMsgWrite(pl_DG_gl_Ch,
									 XDM_IdentMsg(XDM_EMES_6gl_Voie),
									 XDM_FLG_SRVMSGWRITE,
									 T_IPC_FT_REAL8, vl_Mes_Virtuelle.Horodate,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Numero,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Periode,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Sens,
									 T_IPC_FT_INT2, XDC_RADT_CHAUSSEE,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValQ.Val,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValQ.Indic,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValV.Val,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValV.Indic,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValTO.Val,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValTO.Indic,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValPL.Val,
									 T_IPC_FT_INT2, vl_Mes_Virtuelle.Mesures.ValPL.Indic,
									 NULL))
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EInit_IHM_6gl_Chaussee non effectue");
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EInit_IHM_6gl_Chaussee effectue");
				}

				/*B Envoi du message */
				TipcSrvFlush();

				/* Calcul des niveau de service */
				if (!(pl_ListeRADT->Etat_Service))
					er_Niveau_Service(vl_Mes_Virtuelle.Numero, ERDTC_CHAUSSEE, va_Index, vl_Mes_Virtuelle.Horodate, ERDTC_MINUTE);

			} /* fin if */
			else
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "VIRT Stockage des mesures par voie et par chaussee");
				/*B Stockage des mesures par voie et par chaussee */
				memcpy(&(pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_Mes_Virtuelle.Numero]][vl_Mes_Virtuelle.Sens][0][va_Index]),
					   &vl_Mes_Virtuelle,
					   sizeof(ERDT_EMes_RADT));
				XZST_03EcritureTrace(XZSTC_WARNING, "%d", vl_Mes_Virtuelle.Numero);
				er_Env_Mes_6mn(vl_Mes_Virtuelle.Numero, va_Index);
				if (er_Stockage_Mesures(vl_Mes_Virtuelle.Numero, va_Index, vl_Mes_Virtuelle.Horodate, XDC_FAUX) != XDC_OK)
					XZST_03EcritureTrace(XZSTC_WARNING, "%s : 6min Virtuelle : er_Stockage_Mesures retourne NOK", version);

				/* Calcul des niveau de service sur mesure 6 minutes si PC simplifie */
				if (vg_Mac_Ext_Ouest && !(pl_ListeRADT->Etat_Service))
					er_Niveau_Service(vl_Mes_Virtuelle.Numero, ERDTC_CHAUSSEE, va_Index, vl_Mes_Virtuelle.Horodate, ERDTC_6MINUTE);
			}
		} /* fin if */
	}	  /* fin for */

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne OK", version);
	return (XDC_OK);
}
