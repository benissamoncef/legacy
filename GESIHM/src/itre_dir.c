/* Fichier : $Id: itre_dir.c,v 1.80 2021/06/16 08:10:49 pc2dpdy Exp $        $Revision: 1.80 $        $Date: 2021/06/16 08:10:49 $
 *************************************************
 * STERIA * Projet MIGRAZUR			*
 *************************************************
 * MODULE  : MTRE				*
 * FICHIER : itre_dir.c				*
 *************************************************
 * DESCRIPTION :
 * module directeur de la tache relais
 *
 *************************************************
 * HISTORIQUE :
 *
 * J.M. Guilhou	29 Sep 1994 : Creation 			V1.1
 * Guilhou	15 dec 1994 : modif arret_tache		V1.5
 * Mismer	27 dec 1994 : rien		                V1.8
 * Guilhou	27 dec 1994 : lancement locaux techniques	V1.9
 * Noel		28 dec 1994 : ajout abonnement messages TFM 	V1.11
 * T.Milleville	06 Fev 1995 : Synchronisation avec le noyau de SUPERV
 * B.Noel	07 Fev 1995 : Ajout d'un sleep pour lancer mce            V1.18
 * Volcic	23 Mar 1995 : Changement pour LT                          V1.19
 * Lecrivain	02 Avr 1995 : Tache relais unique                         V1.21
 * Lecrivain	07 Avr 1995 : Remplacement des "Printf" par XZST_03       V1.24
 * Lecrivain	07 Avr 1995 : Lancement du script de nettoyage a l'init.  V1.25
 * Lecrivain	13 Avr 1995 : Changement nom du script nettoyage          V1.26
 * Volcic	01 Aou 1995 : Ajout abonnement datagroup IAX_TEXTE_<NomSite>  V1.27
 * Noel		04 Sep 1995 : Changement de la variable dg sur abonnement PAU	V1.28
 * Niepceron	05 Oct 1995 : Ajout de la copie de itex_param en ax_prof3	V1.30
 * Niepceron     12 Jan 1996 : Modification de la copie de l'ax_prof3 pour applix4 devient ax_prof4 (634)       V1.31
 * Torregrossa	17 Jan 1996 : Les messages systemes sont rediriges vers l'applix mce au lieu de dial V1.32
 * Noel		19 Fev 1996 : Passage du type d'imprimante en variable globale pour envoi dans itre_rtw_initMce
 *				et dans itre_rtw_lanceDial (DEM91)							V1.33
 * Volcic	11 Mar 1996 : Ajout desabonnement datagroup inutile pour les LT	(DEM/1094)  V1.34
 * Guilhou	22 aut 1996 : Ajout abonnement XDG_IRAF_FMCLOC_site et XDG_IAX_FINPOSTE_site (RADT) V1.35
 * Niepceron	27 Sep 1996 : Ajout abonnement XDG_IAX_ACQT (dem/424) v1.36
 * Niepceron	29 Sep 1997 : Ajout du plock sauf LT (v1.37)
 * Pour que plock est une chance de reussir : il faut permettre au groupe du user migrazur
 * d avoir le privilege MLOCK : on cree un fichier /etc/privgroup contenant le nom du groupe
 * suivi du (ou des) privilege a donner au groupe en question.
 * pour que le systeme le prenne en compte : lancer setprivgrp -f /etc/privgroup
 * Pour Verifier que le group ait bien le privilege : getprivgrp migrazur_grp
 * Penot		07 Nov 1997 : itre_processEvt_Strada
 * Guilhou	07 jan 1998 : dire que l'operateur n'est plus la sur PC niv 2 aussi (dem/1535) 1.39
 * Guilhou	07 jan 1998 : abonnement dg demande init presence operateur sur PC niv 2 aussi (dem/1535) 1.40
 * Guilhou	12 jan 1998 : dire que l'operateur n'est plus la sur tout PC (dem/1535) 1.41
 * Mismer	12 Fev 1998 :  jout abonnement datagroup IAX_TEXTE_<site> pour PC2 et 3 (dem/1542) 1.42
 * Guilhou		    : abt INITPRES pas tout le temps 1.43 44
 * Niepceron	15 jun 1998 : abt a la surchage au CI (DEM/1691) 1.44
 * Niepceron     21 Dec 1998 : Limitation des envois INITPRES 1.45
 * ESCOTA	03 Mai 1999 : suppression trace itre_dir_traiter_message_ihm 1.46
 * CLUCHAGUE     16 Dec 2003 : Pour eviter que dans les pc de niveau 2 le 2eme poste non logue ecrase au niveau du ci le fait
			      que le 1er poste est logue (mauvaise icone au nivzeau du bandeau)  si on est 2eme poste des pc 
			      de niveau 2 (PODC2,PODS2,PODA2) on ne repond plus aux requetes  1.47 REPERE1
 * JMG		22/10/07 : ajout XDG_IAX_TEXTE_CI au CI egalement (gestion basculement des RAU) 1.48

 * Niepceron	14/01/2008	: Modifier les traitements et filtres specifiques DN pour les adapter  DM DEM743 1.49
 * Lecrivain	17/09/2008 : Abonnement aux messages d'alarme site (pour gestion de l'etat du Pirau) (DEM 808) 1.50
 * JMG           29/09/09    : SECTO DEM 887 1.51
 * JMG    10/02/10        : portage Linux dem/934 1.52
 * JMG	21/12/10	: lancement applix a gauche sur machine linux 1.53
 * JMG 24/0311	: alerte localisee 1.54 DEM 980
 * JPL	23/12/11 : Utilisation de symboles de constantes et primitives Migrazur  1.55
 * JPL	26/12/11 : Abonnement aux messages de timer TdP sur postes radio (DEM 1007)  1.56
 * JMG	04/01/12 : gestion bi ecran pour linux 1.57
 * JMG	04/07/12 : tidbg 1.58
 * PNI	15/10/12 : Sur VD, abt des presences operateurs v1.60 DEM1055
 * JPL	12/11/12 : Support interface avec l'IHM Java / Eclipse RCP  1.61
 * PNI	15/10/12 : remise en place de l'abt sur XDG_IAX_TEXTE DEM1055 v1.62
 * JPL	03/12/12 : Support reception messages multiples (pour interface ASCII avec l'IHM Java / Eclipse RCP)  1.63
 * PNI	04/09/13 : Sur CA, abt des presences operateurs v1.64 DEM1079
 * JMG 	24/02/14 : abt XDG_IMODIF_ALARMES_CRIT 1.65
 * PNI	06/10/14 : Suppression du TIMER TDP 1.67
 * PFE    2/12/14 : modif pour tidbg.x (ecran pour 3eme par) + remise en route du timer 1.68
 * PNI	15/12/14 : Mise en place du timer pour activer socket DEM1116 1.69
 * PFE    21/02/17 : si nom tache = titltdbg.x : 1.70
 * JMG	29/10/18 : ajout cb RMVI DEM1315 1.72
 * RGR   26/03/19 : DEM1331 - Ajout Mecanisme keep alive 1.73
 * JMG	26/06/19 : ajout INFOMESSAGE DEM1338 ODI-C 1.74
 * JMG	17/11/19 ; TDP surcharge DEM1365 1.75
 * JPL	12/02/20 : Support du numero d'ecran en troisieme parametre  1.76
 * JMG	22/05/20  : EMS 1.77
 * GR   10/07/20  :EMS 1.78
 * ABE 26/01/20 : Ajout gestion fichier de config TOPS, pour IP:DISPLAY.PORT pour l'IHM DEM-SAE228 V1.78
 * JMG	10/02/21 : mode Magistra 1.79
 * LCL   15/06/21        : Fix SEGFAULT sur reception RMVI SAE-295 1.80
 * ABE  17/12/2021      : Ajout des app externe appelee depuis magistra SAE311  1.81
 * LCL		20/12/21	: MOVIS P2 ems SAE-312
 * LCL    14/01/22  : Ajout nom machine pour appel itre_ax_processExec dans itre_dir_traiter_message_ihm SAE-312
 * LCL    29/06/22  : Correction taille vl_str dans itre_processExecTitre pour pouvoir traiter les chaines en provenance de la GTC (10000 caractères max)
 * KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
 * LCL		27/09/23 : Adaptations CNA DEM-483	
 * LCL  30/10/23  : Correction longueur variable nom machine dans itre_processExecTitre
***********************************************/

#ifdef _HPUX_SOURCE
#include <sys/lock.h>
#endif
#include <xzsem.h>
#include <errno.h>
#include "itre_dir.h"
#include "itre_ax.h"
#include "itre_rtw.h"
#include "xzems.h"


#define egale(s1, s2)	(!strcmp ((s1), (s2)))


/* definition des variables globales a l'application */
XDY_NomTache	vg_nomTache;
XDY_NomMachine	vg_nomMachine;
XDY_NomSite	vg_nomSite;
int		vg_type_imprimante = 0;
int		vg_type_machine=0;
int		vg_numero_site=0;

XZSCT_Datagroup vg_dg_dial;
XZSCT_Datagroup vg_dg_mce;
XZSCT_Datagroup vg_dg_admin_poste, vg_dg_admin_site;
XZSCT_Datagroup vg_dg_oper_poste, vg_dg_oper_site;
XZSCT_Datagroup vg_dg_dem_surcharge, vg_dg_rep_surcharge;

int 		vg_socRecFromAxMce = -1;
int		vg_socEnvToAxMce = -1;
int 		vg_socRecFromAxDial = -1;
int		vg_socEnvToAxDial = -1;
T_INT4		vg_socRT = -1;

char		vg_display[24] = "";
int		vg_num_display = 0;
int 		vg_numSocket;
int		vg_appli_est_mono_ecran = FALSE;
int		vg_appli_est_debug = FALSE;
int		vg_env_ihm_jer = FALSE;
int		vg_loginDone=TRUE;
int		vg_operateurPresent=FALSE;
int		vg_titre = FALSE;

/* declaration de fonctions internes */
int itre_lire_config_TOPS(XDY_Texte va_param, XDY_Texte va_IP, XDY_Entier *va_display, XDY_Entier *va_port);


void itre_processExecTitre(
    T_IPC_CONN			va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA	pa_data,
    T_CB_ARG			pa_arg)
{
/*#ifndef _TIBCO_EMS
 T_INT4 vl_num;
 T_STR vl_str;
 T_STR vl_nomMachine;
#else*/
 XDY_Entier vl_poke;
 XDY_Entier vl_tar01;
 XDY_Entier vl_exec;
 XDY_Entier vl_num;
 XDY_Entier vl_cmd;
 char vl_str[11000];
 char vl_nomMachine[50];
/*#endif
*/

if (!TipcMsgRead(pa_data->msg, 
    T_IPC_FT_INT4,&vl_poke,
    T_IPC_FT_INT4,&vl_tar01,
    T_IPC_FT_INT4,&vl_exec,
    T_IPC_FT_INT4,&vl_cmd,
		T_IPC_FT_INT4,&vl_num,
		T_IPC_FT_STR,&vl_str,
		T_IPC_FT_STR,&vl_nomMachine,NULL))
{
  XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_processExecTitre");
    return;
    }
  XZST_03EcritureTrace (XZSTC_WARNING, "itre_processExecTitre : vl_cmd = [%d] vl_num = [%d] vl_str(fct) = [%s] vl_nomMachine = [%s] ",vl_cmd, vl_num, vl_str, vl_nomMachine);

  itre_ax_processExec(vl_cmd,vl_num,vl_str,vl_nomMachine);
}

/*-----------------------------------------------------------------------------
 * SERVICE RENDU :
 * Fonction (callback) de traitement des messages generiques
 * (Join, Poke, Newtask, Setvar, ...)
-------------------------------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void itre_processGenericMsg(
    T_IPC_CONN			va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA	pa_data,
    T_CB_ARG			pa_arg)
{
 
  XDY_Datagroup   	vl_dg_dest;
  XDY_Datagroup 	vl_dg;
   char *vl_ch;

  if (!vg_titre)
  {
  if (vg_loginDone==FALSE)
    return;
  }

  /* Determiner le destinataire */
  strcpy(vl_dg_dest,"");
  TipcMsgGetDest(pa_data->msg, &vl_dg_dest);

  XZST_03EcritureTrace (XZSTC_WARNING, "message generique dg %s\n", vl_dg_dest);
  XZST_03EcritureTrace (XZSTC_DEBUG1, "message generique dg %s\n", vl_dg_dest);

  if (egale(vl_dg_dest, vg_dg_admin_site) ||
      egale(vl_dg_dest, vg_dg_admin_poste))
  {
    itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
  } else if (egale(vl_dg_dest, vg_dg_dial))
  {
    itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxDial);
  } else {
    itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
  }
}

/*-----------------------------------------------------------------------------
 * SERVICE RENDU :
 * fonctions (callbacks) de traitement de messages specifiques
-------------------------------------------------------------------------------
 * SEQUENCE D'APPEL :
 */
void itre_rtw_presence(XZAET_EPresence_Op_PCsimpl    va_presence)
{
  itre_rtw_presence_PCS(va_presence);
}

void itre_processTFM(
    T_IPC_CONN			va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA	pa_data,
    T_CB_ARG			pa_arg)
{

  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process tfm\n");
  itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processListeTFM(
    T_IPC_CONN			va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA	pa_data,
    T_CB_ARG			pa_arg)
{

  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process liste tfm\n");
  itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processPA(
    T_IPC_CONN			va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA	pa_data,
    T_CB_ARG			pa_arg)
{

  XZST_03EcritureTrace (XZSTC_WARNING, "itre_processPA : IN");
  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process liste pa\n");
  itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
  XZST_03EcritureTrace (XZSTC_WARNING, "itre_processPA : OUT");
}

void itre_processAlerte(
    T_IPC_CONN			va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA	pa_data,
    T_CB_ARG			pa_arg)
{

  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process alerte");
  itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processAlerteLocalisee(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{

  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process alerte localisee");
  itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processEvtStrada(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{

  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process evenement Strada");
  itre_ax_processMsg(va_cnx, pa_data , pa_arg, vg_socEnvToAxMce);
}


void itre_processFMC(
    T_IPC_CONN			va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA	pa_data,
    T_CB_ARG			pa_arg)
{

  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process modif fmc");
  itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processArch(
    T_IPC_CONN			va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA	pa_data,
    T_CB_ARG			pa_arg)
{

  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process archivage\n");
  itre_ax_processMsg(va_cnx, pa_data, pa_arg, vg_socEnvToAxDial);
}

#ifndef _TIBCO_EMS
void itre_processFMCLoc(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process liste FMC locales");
  itre_rtw_processFMCLoc(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}
#endif
void itre_processRMVI_status(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  itre_rtw_processRMVI_status(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}
void itre_processRMVI_retour(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  itre_rtw_processRMVI_retour(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}
void itre_processRMVI_EMS_retour(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  itre_rtw_processRMVI_EMS_retour(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}
void itre_processALA(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process alarme\n");
  itre_rtw_processALA(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processEQT(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process etat equipement");
  itre_rtw_processEQT(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processFIN(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process demande fin de poste");
  itre_rtw_processFIN(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processTDP(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process TDP");
  itre_rtw_processTDP(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}
void itre_processACQT(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process demande d'acquittement");
  itre_rtw_processACQT(va_cnx, pa_data, pa_arg, vg_socEnvToAxMce);
}

void itre_processAlarme(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message d'alarme");
  itre_rtw_processAlarme(va_cnx, pa_data, pa_arg);
}

void itre_process_Timer_TdP(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{
  if (vg_loginDone==FALSE)
    return;
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message timer TdP");
  itre_rtw_process_Timer_TdP(va_cnx, pa_data, pa_arg);
}

void itre_process_Timer_KEEPALIVE(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message timer KEEPALIVE");
  itre_rtw_process_Timer_KEEPALIVE(va_cnx, pa_data, pa_arg);
}

void itre_process_del_sema(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message delete semaphore");
  itre_rtw_process_del_sema(va_cnx, pa_data, pa_arg);
}
void itre_process_pra_po(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message PRA PO");
  itre_rtw_process_pra_po(va_cnx, pa_data, pa_arg);
}
void itre_process_conf_modern(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message conf modern");
  itre_rtw_process_conf_modern(va_cnx, pa_data, pa_arg);
}
void itre_process_remorque(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message fct annexes");
  itre_rtw_process_fct_remorque(va_cnx, pa_data, pa_arg);
}
void itre_process_remorque_CA(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message fct annexes");
  itre_rtw_process_fct_remorque_CA(va_cnx, pa_data, pa_arg);
}
void itre_process_viabilite_hivernal(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message viabilite hivernal");
  itre_rtw_process_viabilite_hivernal(va_cnx, pa_data, pa_arg);
}
void itre_process_viabilite_hivernal_ASF(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message viabilite hivernal");
  itre_rtw_process_viabilite_hivernal_ASF(va_cnx, pa_data, pa_arg);
}
void itre_process_viabilite_hivernal_export(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message viabilite hivernal");
  itre_rtw_process_viabilite_hivernal_export(va_cnx, pa_data, pa_arg);
}
void itre_process_lt_alarme(T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
  XZST_03EcritureTrace (XZSTC_DEBUG1, "process message lt alarme");
  itre_rtw_process_lt_alarme(va_cnx, pa_data, pa_arg);
}

void itre_process_init_titre (T_IPC_CONN va_cnx,
                                  T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                  T_CB_ARG pa_arg)
{
XDY_NomMachine               vl_machine="";
XDY_Datagroup vl_dg;

if (!TipcMsgRead(pa_data->msg, T_IPC_FT_STR,&vl_machine,NULL))
{
  XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_process_init_titre");
  return;
}

  XZST_03EcritureTrace (XZSTC_FONCTION, "itre_process_init_titre: machine %s",vl_machine);
  XDG_EncodeDG2(vl_dg, XDG_IAX_SYNOP, vl_machine);
  XDG_JoindreDG(vl_dg);
  XDG_EncodeDG2(vl_dg, XDG_IAX_TEXTE_EMS, vl_machine);
  XDG_JoindreDG(vl_dg);
  XDG_EncodeDG2(vl_dg, XDG_IAX_TEXTE, vl_machine);
  XDG_JoindreDG(vl_dg);

  itre_rtw_initMce(vl_dg, vl_machine);

  XDG_EncodeDG2(vl_dg, XDG_IMESSAGE_TFM_PRET, vl_machine);
  XDG_JoindreDG(vl_dg);
  XDG_EncodeDG2(vl_dg, XDG_IPA_PRET, vl_machine);
  XDG_JoindreDG(vl_dg);
  XDG_EncodeDG2(vl_dg, XDG_IMODIF_FMC, vl_machine);
  XDG_JoindreDG(vl_dg);
  XDG_EncodeDG2(vl_dg, XDG_IAVERT_OPERATEUR, vl_machine);
  XDG_JoindreDG(vl_dg);
  
}

void itre_processInitPresence(
    T_IPC_CONN                      va_cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_data,
    T_CB_ARG                        pa_arg)
{

#ifndef _TIBCO_EMS
  T_STR pl_Emetteur    = "\0";
#else
 XDY_Nom  pl_Emetteur    = "\0";
#endif

  TipcMsgGetSender(pa_data->msg,&pl_Emetteur);
  XZST_03EcritureTrace(XZSTC_WARNING,"JMG %s",pl_Emetteur);

  /* REPERE1 */
  if ((vg_numero_site != 0) && 
      /*     (strncmp (vg_nomMachine,"PODC2",5) != 0 ) &&
      (strncmp (vg_nomMachine,"PODA2",5) != 0 ) &&
      (strncmp (vg_nomMachine,"PODS2",5) != 0 ) &&*/
      ((vg_type_machine==XDC_TYPEM_ST) || 
          (vg_type_machine==XDC_TYPEM_PCS) ||
          (vg_type_machine==XDC_TYPEM_SC) ||
          (vg_type_machine==XDC_TYPEM_PC2) ||
          (vg_type_machine==XDC_TYPEM_PC2LT)))
  {
    if (vg_operateurPresent==FALSE)
      TipcSrvMsgWrite(XDG_A_PRES_OP,
          XDM_IdentMsg(XDM_A_PRES_OP),
          FALSE,
          T_IPC_FT_INT2,vg_numero_site,
          T_IPC_FT_INT2,XDC_FAUX,
          T_IPC_FT_STR,vg_nomMachine,
          NULL);
    else
      TipcSrvMsgWrite(XDG_A_PRES_OP,
          XDM_IdentMsg(XDM_A_PRES_OP),
          FALSE,
          T_IPC_FT_INT2,vg_numero_site,
          T_IPC_FT_INT2,XDC_VRAI,
          T_IPC_FT_STR,vg_nomMachine,
          NULL);
    TipcSrvFlush();
  }
  /*trace*/
  XZST_03EcritureTrace(XZSTC_FONCTION,
      "itre_processInitPresence: emission du message XDM_A_PRES_OP(%d) sur le dg XDG_A_PRES_OP",
      vg_operateurPresent ? XDC_VRAI : XDC_FAUX);
  return;
}


int itre_dir_init(
    XDY_NomTache	va_nomTache,
    char		*va_nomAppli)

{
 XDY_Datagroup vl_dg;
  int	vl_ValRet; /* compte-rendu d'appel */
  char vl_commande[250];
  XZSCT_NomPeriph vl_NomImprimante="";
  XDY_Booleen	code_retour;
  XZSMT_CleProg	vl_cle_timer;
  XDY_Texte va_IP;
  XDY_Entier va_display,va_port;
  int vl_indice;
  XDY_NomMachine               vl_po_site="";

  /****  Connexion au RTServer ***/
  /* Arguments : Nom appli, nom tache,
   *	  Autorisation de MAJ date, de tracer, de synchro avec SUPERV,
   *	  Fonction de fin de tache (XDM_TSP_EXIT), redirection sortie standard
   */
  XZSC_01InitCnxSup (va_nomAppli,va_nomTache,
      XZSCC_INTERDIRE, XZSCC_AUTORISER,
      /*egale(va_nomTache, ITREC_RELAIS_LT) ? XZSCC_INTERDIRE : XZSCC_AUTORISER,*/
      XZSCC_INTERDIRE,
      NULL, NULL);

  /*A Determination des noms de la machine et du site
   *  sur lesquels s'execute la tache */
  if ((vl_ValRet = XZSC_07NomMachine(vg_nomMachine)) != XDC_OK  ||
      (vl_ValRet = XZSC_04NomSite(vg_nomSite)) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "Recuperation nom machine ou site a echoue (%d)\n", vl_ValRet);
    return (XDC_NOK);
  }
  
  /* recuperation du type de ma machine*/
  if ((vl_ValRet = XZSA_17TypeMachine(vg_nomMachine,&vg_type_machine)) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "Recuperation type machine a echoue (%d)\n", vl_ValRet);
    return (XDC_NOK);
  }

  /*A Recuperation du nom d'imprimante de la station */
  if ((vl_ValRet = XZSC_11NomImprimante(vl_NomImprimante)) != XDC_OK )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "Recuperation nom imprimante a echoue (%d)\n", vl_ValRet);
  }
  /*A Recuperation du type d'imprimante de la station */
  if ((vl_ValRet =XZSA_26TypeEntite(vl_NomImprimante, &vg_type_imprimante )) != XDC_OK )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "Recuperation type imprimante a echoue (%d)\n", vl_ValRet);
  }

  switch (vg_type_machine) {

  case XZSCC_TYPEM_ST :
  case XZSCC_TYPEM_SD :
  case XZSCC_TYPEM_SC :
  case XZSCC_TYPEM_PCS :
    XZST_03EcritureTrace(XZSTC_INFO,
        "Sur le type de machine %d, on lock en memoire", vg_type_machine);

    /*B OM : En mode operationnel, on essaie de locker en mem titxx */
    errno = 0;
#ifdef _HPUX_SOURCE
    if ((code_retour = plock (PROCLOCK))== -1)
    {
      /*Erreur detectee pour locker la totalite de la tache en memoire*/
      perror("Impossible de locker la totalite de la tache en memoire vive :");
      XZST_03EcritureTrace(XZSTC_WARNING,"plock : Impossible de locker la totalite de la tache en memoire vive : code de retour : %d",code_retour);
    }
#endif
    break;

  default :
    XZST_03EcritureTrace(XZSTC_INFO,
        "Sur le type de machine %d, on ne lock pas en memoire", vg_type_machine);
  }


  /**** Suppression des eventuelles taches Applix subsistant ****/
  if (!vg_titre)
  system("titclean.ksh");

  /*A Recuperation du bon fichiers pour le copier en  axhome/ax_prof4 suivant le type d'imprimante ****/

  switch (vg_type_imprimante)
  {
  case XZSAC_TYPE_IMP_NB:
    sprintf( vl_commande , "cp %s %s/axhome/ax_prof4",  ITR_AXPROF_NB , XDC_PATH_MIGRAZURHOME );
    break;
  case XZSAC_TYPE_IMP_CL:
    sprintf( vl_commande , "cp %s %s/axhome/ax_prof4",  ITR_AXPROF_CL , XDC_PATH_MIGRAZURHOME );
    break;
  default:
    XZST_03EcritureTrace(XZSTC_WARNING, "Type de commande inconnu:%d; par defaut imprimante N&B (PCL5)",vg_type_imprimante);
    sprintf( vl_commande , "cp %s %s/axhome/ax_prof4",  ITR_AXPROF_NB , XDC_PATH_MIGRAZURHOME );
    break;
  }

  if (!vg_titre)
  {
  vl_ValRet = system( vl_commande );
  if ( vl_ValRet != 0 )
    XZST_03EcritureTrace(XZSTC_WARNING, "%s : cr=%d\n", vl_commande ,vl_ValRet);

  /**** Lancement d'Applix sur un ou deux ecran(s) ****/
  vg_numSocket = time((time_t *) NULL) % 1000;
  if (vg_env_ihm_jer  &&  vg_appli_est_debug)
    vg_numSocket = 0;

  if (vg_env_ihm_jer)
  {
    sprintf(vl_commande, CMDAPPLIJER, vg_display, 0,
        ITREC_NUMPORT_MCE + vg_numSocket, vg_appli_est_mono_ecran ? 2 : 0);
  }
  else
  {
    /* Si on est pas en debug et que le fichier TOP est renseigné correctement */
    if( (!vg_appli_est_debug) && (itre_lire_config_TOPS(CO_FIC_TOPS_DISPLAY1, va_IP, &va_display, &va_port)==XDC_OK) )
    {
      sprintf(vl_commande, CMDAPPLIX, va_IP, va_display, va_port, ITREC_NUMPORT_MCE + vg_numSocket,  vg_appli_est_mono_ecran ? 2 : 0);							
    }
    else
    {
      sprintf(vl_commande, CMDAPPLIX, vg_display, vg_num_display, 0, ITREC_NUMPORT_MCE + vg_numSocket,  vg_appli_est_mono_ecran ? 2 : 0);
    }
  }

  XZST_03EcritureTrace (XZSTC_WARNING, "%s\n", vl_commande);
  vl_ValRet = 10;
  if (!(vg_env_ihm_jer  &&  vg_appli_est_debug))
    vl_ValRet = system(vl_commande);
  XZST_03EcritureTrace(XZSTC_WARNING, "Lancement IHM : cr=%d\n", vl_ValRet);

  sleep(5);

  if (!vg_appli_est_mono_ecran)
  {
    if (vg_env_ihm_jer)
      sprintf(vl_commande, CMDAPPLIJER, "", 1, ITREC_NUMPORT_DIAL + vg_numSocket, 1);
    else
    {
      /* Si on est pas en debug et que le fichier TOP est renseigne correctement */
      if ( (!vg_appli_est_debug) && (itre_lire_config_TOPS(CO_FIC_TOPS_DISPLAY2, va_IP, &va_display, &va_port)==XDC_OK) )
      {
         sprintf(vl_commande, CMDAPPLIX, va_IP, va_display, va_port, ITREC_NUMPORT_DIAL + vg_numSocket, 1);					
      }
      else
      {
         sprintf(vl_commande, CMDAPPLIX, "", vg_num_display, 1, ITREC_NUMPORT_DIAL + vg_numSocket, 1);
      }
    }
      XZST_03EcritureTrace (XZSTC_WARNING, "%s\n", vl_commande);
      vl_ValRet = 10;
      if (!(vg_env_ihm_jer  &&  vg_appli_est_debug))
        vl_ValRet = system(vl_commande);
      XZST_03EcritureTrace(XZSTC_WARNING,"Lancement IHM : cr=%d\n",vl_ValRet);
  }
  }


  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_INTERFACE|XZSTC_FONCTION,XZSTC_NECRAN);

  /**** Declaration des DataGroups connus du coupleur*/
  XDG_EncodeDG2 (vg_dg_mce, XDG_IAX_TEXTE, vg_nomMachine);
  XDG_EncodeDG2 (vg_dg_dial, XDG_IAX_SYNOP, vg_nomMachine);

  /*recuperation du fd de connexion RTworks*/

XZST_03EcritureTrace(XZSTC_WARNING,"ici");
#ifndef _TIBCO_EMS
  TipcSrvGetSocket(&vg_socRT);

  /*declaration des callbacks sur message de reception des FMC locales*/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IRAF_FMCLOC),
      itre_processFMCLoc,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }
#endif

XZST_03EcritureTrace(XZSTC_WARNING,"la");
  /*declaration des callbacks sur message de demande de fin de poste*/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAX_FINPOSTE),
      itre_processFIN,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (vg_titre)
  {
    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_INIT_TITRE),
      itre_process_init_titre,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }

    /* Abonnement au data group MAGISTRA REMOTE CMD */
    XDG_EncodeDG2(vl_dg, XDG_MGA_REMOTE_CMD, vg_nomSite);
    XDG_JoindreDG(vl_dg);

    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_DEL_SEMA),
      itre_process_del_sema,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }

    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_CFG_PRA_PO),
      itre_process_pra_po,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }

    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_LIVRE_MODERNE),
      itre_process_conf_modern,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }

    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_JAR_REMORQUE),
      itre_process_remorque,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }
    
    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_JAR_REMORQUE_CA),
      itre_process_remorque_CA,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }

    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_HIVERNAGE),
      itre_process_viabilite_hivernal,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }

    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_HIVERNAGE_ASF),
      itre_process_viabilite_hivernal_ASF,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }

    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_HIVERNAGE_EXPORT),
      itre_process_viabilite_hivernal_export,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }
    
    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_MGA_CMD_ALARME_LT),
      itre_process_lt_alarme,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }
  }

  /*boutons alarme/eqt*/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IPOSIT_INDIC_ALARME),
      itre_processALA,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IETAT_DISPO_EQT),
      itre_processEQT,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_RMVI_STATUS),
      itre_processRMVI_status,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_RMVI_RETOUR_EMS),
      itre_processRMVI_EMS_retour,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }
  if (!vg_titre)
  {
    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_RMVI_RETOUR),
        itre_processRMVI_retour,(T_CB_ARG) NULL)==NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }
  }
  /*declaration des callbacks sur message de reception de demande d'acquittemnt*/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAX_ACQT),
      itre_processACQT,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ETDP_ETAT_SURCHARGE),
      itre_processTDP,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  /*declaration des callbacks sur message d'alarme*/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AM_alarme),
      itre_processAlarme,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }


  /*declaration des callbacks sur messages modif fmc*/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IMODIF_FMC),
      itre_processFMC,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }



  /*declaration des callbacks sur messages trafic fm, plan d'action et alerte*/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IMESSAGE_TFM_PRET),
      itre_processTFM,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IPA_PRET),
      itre_processPA,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ICREER_ALERTE),
      itre_processAlerte,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ICREER_ALERTE_LOCALISEE),
      itre_processAlerteLocalisee,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAEVT_STRADA),
      itre_processEvtStrada,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IMODIF_LISTE_TFM),
      itre_processListeTFM,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ITDP_TIMER),
      itre_process_Timer_TdP,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  /* Abonnement au data group keep alive */
  XDG_EncodeDG2(vl_dg, XDG_KEEPALIVE, vg_nomSite);
  XDG_JoindreDG(vl_dg);

  /* N'utilise pas le service generique car fait appel a la socket axnet */
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_KEEPALIVE),
      itre_process_Timer_KEEPALIVE,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }


  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IARCHIVAGE),
      itre_processArch,(T_CB_ARG) NULL)==NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (vg_titre)
  {
    if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EXEC_TITRE),
        itre_processExecTitre,(T_CB_ARG) NULL) == NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
      return (vl_ValRet);
    }
  }

  /** Declaration d'un Callback
   *	Sur reception du Message XDM_IAX_xxxx(4301 a 4304)   ***/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAX_JOINDG),
      itre_processGenericMsg,(T_CB_ARG) NULL) == NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAX_SENDPOKE),
      itre_processGenericMsg,(T_CB_ARG) NULL) == NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAX_NEWTASK),
      itre_processGenericMsg,(T_CB_ARG) NULL) == NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAX_SETVARCTX),
      itre_processGenericMsg,(T_CB_ARG) NULL) == NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAX_INFOMESSAGE),
      itre_processGenericMsg,(T_CB_ARG) NULL) == NULL)
  {
    vl_ValRet = XZSCC_RTW_INV;
    return (vl_ValRet);
  }


  /*connexion a la socket creee par le module d'emission Applix ITMA_TAR01*/
  if (!vg_titre)
  {
  do
  {
    vl_ValRet = itre_com_Connexion(&vg_socRecFromAxMce,
        ITREC_NUMPORT_MCE + vg_numSocket,
        vg_nomMachine);
    if (vl_ValRet != XDC_OK)
      sleep(1);
  } while (vl_ValRet != XDC_OK);

  /*connexion a la socket creee par le module de reception Applix ITMA_TAR02*/
  do
  {
    vl_ValRet = itre_com_Connexion(&vg_socEnvToAxMce,
        ITREC_NUMPORT_MCE + vg_numSocket + 1,
        vg_nomMachine);
    if (vl_ValRet != XDC_OK)
      sleep(1);
  } while (vl_ValRet != XDC_OK);


  if (vg_appli_est_mono_ecran)
  {
    vg_socRecFromAxDial = -1;
    vg_socEnvToAxDial = vg_socEnvToAxMce;
  }
  else
  {
    /*connexion a la socket creee par le module d'emission cote synoptique */
    do
    {
      vl_ValRet = itre_com_Connexion(&vg_socRecFromAxDial,
          ITREC_NUMPORT_DIAL + vg_numSocket,
          vg_nomMachine);
      if (vl_ValRet != XDC_OK)
        sleep(1);
    } while (vl_ValRet != XDC_OK);

    /*connexion a la socket creee par le module de reception cote synoptique*/
    do
    {
      vl_ValRet = itre_com_Connexion(&vg_socEnvToAxDial,
          ITREC_NUMPORT_DIAL + vg_numSocket + 1,
          vg_nomMachine);
      if (vl_ValRet != XDC_OK)
        sleep(1);
    } while (vl_ValRet != XDC_OK);
  }
  }

  /*joindre le datagroup des messages cote synoptique*/
  XDG_JoindreDG(vg_dg_dial);

  /* ne pas joindre des le debut le datagroup des messages administrateur */
  XDG_EncodeDG2 (vg_dg_admin_site, XDG_IAVERT_ADMINISTRATEUR, vg_nomSite);
  XDG_EncodeDG2 (vg_dg_admin_poste, XDG_IAVERT_ADMINISTRATEUR, vg_nomMachine);

  /*joindre le datagroup de sauvegarde / restauration*/
  XDG_EncodeDG2(vl_dg, XDG_IARCHIVAGE, vg_nomMachine);
  XDG_JoindreDG(vl_dg);

  XDG_EncodeDG2(vl_dg , XDG_IPOSIT_INDIC_ALARME , vg_nomSite );
  XDG_JoindreDG(vl_dg) ;
  XDG_EncodeDG2(vl_dg , XDG_IETAT_DISPO_EQT, vg_nomSite );
  XDG_JoindreDG(vl_dg) ;


  /*joindre le datagroup des messages cote main courante*/
  if (!vg_titre)
  XDG_JoindreDG(vg_dg_mce);

  //XDG_EncodeDG2 (vl_dg, XDG_IAX_TEXTE_EMS, vg_nomSite);
  //XDG_JoindreDG(vl_dg);
  XDG_EncodeDG2 (vl_dg, XDG_IAX_TEXTE, vg_nomSite);
  XDG_JoindreDG(vl_dg);
/* Patch gateway SS/ems */
#ifdef _TIBCO_EMS
//  XDG_EncodeDG2(vl_dg, XDG_IAX_TEXTE_EMS, vg_nomMachine);
#else
//  XDG_EncodeDG2(vl_dg, XDG_IAX_TEXTE_EMS_SS, vg_nomMachine);
#endif
//  XDG_JoindreDG(vl_dg);



  /* Sur les postes radio, joindre les datagroups de rafraichissement */
  /* de la liste des messages TFM  */
  XDG_JoindreDG(XDG_IMODIF_LISTE_TFM);

  XDG_EncodeDG2(vl_dg,XDG_IMODIF_ALARMES_CRIT,vg_nomSite);
  XDG_JoindreDG(vl_dg);

  /*joindre le datagroup des messages d'alarme pour etat du Pirau */
  XDG_EncodeDG2 (vl_dg, XDG_AM, vg_nomSite);
  XDG_JoindreDG(vl_dg);

  /* Joindre les datagroups trafic FM, plan d'action et alerte */
  if (!vg_titre) {
  XDG_EncodeDG2 (vl_dg, XDG_IMESSAGE_TFM_PRET, vg_nomMachine);
  XDG_JoindreDG(vl_dg);

  XDG_EncodeDG2 (vl_dg, XDG_IPA_PRET, vg_nomMachine);
  XDG_JoindreDG(vl_dg);
  }

  XDG_EncodeDG2 (vl_dg, XDG_ICREER_ALERTE, vg_nomSite);
  XDG_JoindreDG(vl_dg);

  XDG_EncodeDG2 (vl_dg, XDG_IAEVT_STRADA, vg_nomSite);
  XDG_JoindreDG(vl_dg);

  if (!vg_titre) {
  XDG_EncodeDG2 (vl_dg, XDG_IMODIF_FMC, vg_nomMachine);
  XDG_JoindreDG(vl_dg);
  }

  XDG_EncodeDG2 (vl_dg, XDG_IMODIF_FMC, vg_nomSite);
  XDG_JoindreDG(vl_dg);

  if (vg_titre) {
      XDG_EncodeDG2 (vl_dg, XDG_API, vg_nomSite);
      XDG_JoindreDG(vl_dg);
      
      XDG_EncodeDG2 (vl_dg, XDG_TITRE, vg_nomSite);
      XDG_JoindreDG(vl_dg);      
  }

  /*joindre le datagroup d'avertissement operateur pour ma machine*/
  /* et mon site*/
  if (!vg_titre) {
  XDG_EncodeDG2 (vg_dg_oper_poste, XDG_IAVERT_OPERATEUR, vg_nomMachine);
  XDG_JoindreDG(vg_dg_oper_poste);
  }

  XDG_EncodeDG2 (vg_dg_oper_site, XDG_IAVERT_OPERATEUR, vg_nomSite);
  XDG_JoindreDG(vg_dg_oper_site);

  XDG_EncodeDG2(vg_dg_dem_surcharge, XDG_IDEMANDE_SURCHARGE,XDC_NOM_SITE_CI);
  XDG_EncodeDG2(vg_dg_rep_surcharge, XDG_IREPONSE_SURCHARGE, vg_nomMachine);

  /*si je suis au CI*/
  if (egale(vg_nomSite, XDC_NOM_SITE_CI))
  {
    /*je joins le datagroup de demande de recept evt strada */
    XDG_JoindreDG(XDG_IAEVT_STRADA);
    XDG_JoindreDG(vg_dg_dem_surcharge);

    XDG_EncodeDG2(vl_dg, XDG_IETAT_DISPO_EQT , XDC_NOM_SITE_VC );
    XDG_JoindreDG(vl_dg);
    XDG_EncodeDG2(vl_dg, XDG_IETAT_DISPO_EQT , XDC_NOM_SITE_DP );
    XDG_JoindreDG(vl_dg);
    XDG_EncodeDG2(vl_dg, XDG_IETAT_DISPO_EQT , XDC_NOM_SITE_CA );
    XDG_JoindreDG(vl_dg);
  }
  else {
    /*sinon je joins mon datagroup de reponse a la surcharge*/
    XDG_JoindreDG(vg_dg_rep_surcharge);

    /*joindre le dg de demande de fin de poste*/
    XDG_EncodeDG2 (vl_dg, XDG_IAX_FINPOSTE, vg_nomSite);
    XDG_JoindreDG(vl_dg);
  }

  /*au Pc simplifie*/
  if (vg_type_machine==XZSAC_TYPEM_PCS) {
    /*joindre le dg de reception des FMC locales*/
    XDG_EncodeDG2 (vl_dg, XDG_IRAF_FMCLOC, vg_nomSite);
    XDG_JoindreDG(vl_dg);
  }
  else {
    /*joindre le dg de reception de demande d'acquittement dem/424 */
    XDG_EncodeDG2 (vl_dg, XDG_IAX_ACQT, vg_nomMachine);
    XDG_JoindreDG(vl_dg);
  }

  /*si je suis la tache relais locaux techniques je lance le menu LT*/
  if (egale(va_nomTache, ITREC_RELAIS_LT))
  {
    itre_rtw_initLT(vg_dg_dial);
    XDG_EncodeDG2 (vl_dg, XDG_IAX_TEXTE_EMS, vg_nomSite);
    XDG_QuitterDG(vl_dg);
    XDG_EncodeDG2 (vl_dg, XDG_IAX_TEXTE, vg_nomSite);
    XDG_QuitterDG(vl_dg);
    XDG_EncodeDG2 (vl_dg, XDG_ICREER_ALERTE, vg_nomSite);
    XDG_QuitterDG(vl_dg);
    XDG_EncodeDG2 (vl_dg, XDG_IMODIF_FMC, vg_nomSite);
    XDG_QuitterDG(vl_dg);
    XDG_EncodeDG2 (vl_dg, XDG_IAX_ACQT, vg_nomMachine);
    XDG_QuitterDG(vl_dg);
  }
  else {
    if (egale(va_nomTache, ITREC_RELAIS_LT_DBG))
    {
      itre_rtw_initLT(vg_dg_dial);
      XDG_EncodeDG2 (vl_dg, XDG_IAX_TEXTE, vg_nomSite);
      XDG_QuitterDG(vl_dg);
      XDG_EncodeDG2 (vl_dg, XDG_ICREER_ALERTE, vg_nomSite);
      XDG_QuitterDG(vl_dg);
      XDG_EncodeDG2 (vl_dg, XDG_IMODIF_FMC, vg_nomSite);
      XDG_QuitterDG(vl_dg);
      XDG_EncodeDG2 (vl_dg, XDG_IAX_ACQT, vg_nomMachine);
      XDG_QuitterDG(vl_dg);

    }
    else if (!vg_titre)
    {
      /* sinon initialiser Applix cote main courante (en bi-ecran) */
          if (!vg_appli_est_mono_ecran)
            itre_rtw_initMce(vg_dg_mce, vg_nomMachine);

          /* puis initialiser Applix cote synoptique (ou mono-ecran)
           * et lancer le module de login */
          itre_rtw_lanceDial(vg_dg_dial, "itre_dir");

          /*DEM743 : secteur */
          if ((!egale(vg_nomSite, XDC_NOM_SITE_CI)) || 
              (!egale(vg_nomSite, XDC_NOM_SITE_DP)) || 
              (!egale(vg_nomSite, XDC_NOM_SITE_VC)) || 
              (!egale(vg_nomSite, XDC_NOM_SITE_CA))) /* VH DEM1055 +DEM1079*/
          {
            /*joindre le datagroup d'init de presence de l'operateur*/
            XDG_JoindreDG (XDG_IAX_INITPRES);

            /*armer un callback sur le message de demande d'init*/
            if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IAX_INITPRES),
                itre_processInitPresence,(T_CB_ARG) NULL)==NULL) {
              vl_ValRet = XZSCC_RTW_INV;
              return (vl_ValRet);
            }
          }
    }
  }

  return (XDC_OK);
}

extern int itre_dir_fin_communication()
{
  /*trace*/
  XZST_03EcritureTrace(XZSTC_INFO,"envoi du message XDM_A_PRES_OP(XDC_FAUX) dans le dg XDG_A_PRES_OP");

  /*message de fin de presence de l'operateur*/
  /* REPERE1 */
  if (
      (
          (vg_type_machine==XDC_TYPEM_ST) ||
          (vg_type_machine==XDC_TYPEM_PCS) ||
          (vg_type_machine==XDC_TYPEM_SC) ||
          (vg_type_machine==XDC_TYPEM_PC2) ||
          (vg_type_machine==XDC_TYPEM_PC2LT)
      )
      /*	 &&
            (strncmp (vg_nomMachine,"PODC2",5) != 0 ) &&
            (strncmp (vg_nomMachine,"PODA2",5) != 0 ) &&
            (strncmp (vg_nomMachine,"PODS2",5) != 0 ) 
       */
  )
  {
    TipcSrvMsgWrite(XDG_A_PRES_OP,
        XDM_IdentMsg(XDM_A_PRES_OP),
        FALSE,
        T_IPC_FT_INT2,vg_numero_site,
        T_IPC_FT_INT2,XDC_FAUX,
        T_IPC_FT_STR,vg_nomMachine,
        NULL);
    TipcSrvFlush();
    XZST_03EcritureTrace(XZSTC_WARNING,"envoi du message XDM_A_PRES_OP(XDC_FAUX) dans le dg XDG_A_PRES_OP car je suis : %s et de type : %d",vg_nomMachine,vg_type_machine);
  }


  if (!vg_appli_est_mono_ecran  &&  vg_socEnvToAxDial != -1)
    itre_com_SendPoke(vg_socEnvToAxDial, ITRE_COM_MSG_QUIT, NULL, 0,"");

  if (vg_socEnvToAxMce != -1)
    itre_com_SendPoke(vg_socEnvToAxMce, ITRE_COM_MSG_QUIT, NULL, 0,"");


  /* Arreter tous les reveils actifs */
  itre_Timer_TdP_activer (XDC_FAUX);

  /* Attendre la fin des echanges en cours */
  sleep (1);

#ifndef _TIBCO_EMS
  /* Fermer tous les canaux de communication ouverts */
  if (vg_socRT != -1)
  {
    itre_com_CoupeConnexion(vg_socRT);
    itre_com_FermeSocket(vg_socRT);
  }
#endif


  if (vg_socRecFromAxMce != -1)
  {
    itre_com_CoupeConnexion(vg_socRecFromAxMce);
    itre_com_FermeSocket(vg_socRecFromAxMce);
  }

  if (vg_socEnvToAxMce != -1)
  {
    itre_com_CoupeConnexion(vg_socEnvToAxMce);
    itre_com_FermeSocket(vg_socEnvToAxMce);
  }


  if (!vg_appli_est_mono_ecran)
  {
    if (vg_socRecFromAxDial != -1)
    {
      itre_com_CoupeConnexion(vg_socRecFromAxDial);
      itre_com_FermeSocket(vg_socRecFromAxDial);
    }

    if (vg_socEnvToAxDial != -1)
    {
      itre_com_CoupeConnexion(vg_socEnvToAxDial);
      itre_com_FermeSocket(vg_socEnvToAxDial);
    }
  }

  /*bye*/
  XZST_03EcritureTrace(XZSTC_FONCTION, "Arret tache relais %s\n", vg_nomTache);
}



static int itre_dir_traiter_message_ihm(
    ITRE_COM_TMsgDecode *va_msg)
{
   XZSCT_NomMachine  vl_NomMachine = "\0";
  
   XZST_03EcritureTrace(XZSTC_WARNING,
			"Contenu message IHM : Code:%d Cmd:%d Fct:%d Arg:%s\n",
			va_msg->codeMsg,
			va_msg->contenuMsg.execMsg.numCmd,
			va_msg->contenuMsg.execMsg.numFct,
			va_msg->contenuMsg.execMsg.arg);

   XZSC_07NomMachine(vl_NomMachine);
 
  switch (va_msg->codeMsg)
  {
  /*message de fin de tache*/
  case ITRE_COM_MSG_FIN_TACHE:
    /*on s'arrete*/
    XZST_03EcritureTrace (XZSTC_WARNING, "arret de la tache\n");
    itre_dir_fin_communication();
    exit(0);

    /*message EXEC*/
  case ITRE_COM_MSG_EXEC:
    itre_ax_processExec(va_msg->contenuMsg.execMsg.numCmd,
        va_msg->contenuMsg.execMsg.numFct,
        va_msg->contenuMsg.execMsg.arg,
	      vl_NomMachine);
    break;

    /*message SEND*/
  case ITRE_COM_MSG_SEND:
    itre_ax_processSend(va_msg->contenuMsg.sendMsg.msgType,
        va_msg->contenuMsg.sendMsg.dg,
        va_msg->contenuMsg.sendMsg.arg);
    break;
  }
}



int main(
    int argc,
    char *argv[])
{
  char	*pl_nom;

  /*pour la scrutation des sockets*/
  static fd_set 	vl_fdLire;
  static fd_set 	vl_fdEcrire;
  int 		vl_evts;
  T_IPC_MSG	vl_RTmsg;
  struct timeval montime;

  ITRE_COM_TMsgDecode vl_msg;
  int vl_autreMsg;

  /* Determiner le nom de la tache */
  if ((pl_nom = strrchr(argv[0], '/')) == NULL)
    strcpy(vg_nomTache, argv[0]);
  else strcpy(vg_nomTache, ++pl_nom);


  /* Determiner le mode de fonctionnement mono ou bi-ecran */
  vg_appli_est_mono_ecran = (strncmp(vg_nomTache, ITREC_RELAIS_BI, 4) != 0);
  vg_appli_est_debug = (strstr(vg_nomTache, "dbg") != NULL);
  vg_env_ihm_jer = (vg_nomTache[4] == 'e');
  vg_titre = !(strncmp(vg_nomTache, ITREC_RELAIS_TITRE,4) != 0);

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);


  if (vg_appli_est_debug)
  {
    if (argv[2] != NULL)
      strcpy(vg_display,argv[2]);
    if (argv[3] != NULL)
      vg_num_display = atoi(argv[3]);
  }
  if (vg_titre)
  	vg_env_ihm_jer=FALSE;

  if (vg_env_ihm_jer) {
    itre_com_ModeCommIHM ("ascii");
  } else {
    itre_com_ModeCommIHM ("applix");
  }

  /* Initialisation propre a mon sous systeme */
  if (itre_dir_init(vg_nomTache, XDC_NOM_APPLICATION) != XDC_OK)
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "Erreur a l'initialisation\n");
    itre_dir_fin_communication();
    exit(2);
  }

XZST_03EcritureTrace(XZSTC_WARNING, "vg_titre = %d",vg_titre);

  /* traitement de reception des messages */
  for (;;)
  {
    /*initialise les descripteurs de lecture et d'ecriture*/
    FD_ZERO(&vl_fdLire);
    FD_ZERO(&vl_fdEcrire);

#ifndef _TIBCO_EMS
    FD_SET(vg_socRT, &vl_fdLire);
#endif

    if (!vg_titre)
  {
    FD_SET(vg_socRecFromAxMce, &vl_fdLire);
    if (!vg_appli_est_mono_ecran)
      FD_SET(vg_socRecFromAxDial, &vl_fdLire);
  }

    /*lecture bloquante sur les descripteurs*/
#ifndef _TIBCO_EMS
    vl_evts = select ((size_t) (sizeof(int) << 3),
        (fd_set *) &vl_fdLire, (fd_set *) &vl_fdEcrire, (fd_set *) 0,
        (struct timeval *) NULL);
#else
	montime.tv_sec = 0;
	montime.tv_usec = 500;
	vl_evts = select ((size_t) (sizeof(int) << 3),(fd_set *) &vl_fdLire, (fd_set *) &vl_fdEcrire, (fd_set *) 0,&montime);
#endif
    if (vl_evts < 0)
    {
      if (errno == EINTR)
        continue;

      else if (vl_evts == -1)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"Erreur lors de la scrutation\n");
        itre_dir_fin_communication();
        exit(3);
      }
    }


#ifndef _TIBCO_EMS
    /*si message RTworks*/
    if (FD_ISSET(vg_socRT, &vl_fdLire))
    {
      while (TipcSrvCheck(T_SOCKET_CHECK_READ, 0.0))
      {
        if (TipcSrvRead(0.0))
        {
          vl_RTmsg=TipcSrvMsgNext(0.0);
          while (isMessageNotNULL(vl_RTmsg))
          {
            TipcSrvMsgProcess(vl_RTmsg);
	    TipcMsgDestroy(vl_RTmsg);
          }
        }
      }
    }
#else
	vl_RTmsg = TipcSrvMsgNext(500);
/*	if (vl_RTmsg!= NULL) 	
	{
		TipcSrvMsgProcess(vl_RTmsg);
		TipcMsgDestroy(vl_RTmsg);
	}
*/
#endif
    /* si message d'Applix cote IHM texte (ou mono-ecran) */
    if (FD_ISSET(vg_socRecFromAxMce, &vl_fdLire))
    {
      do {
        XZST_03EcritureTrace (XZSTC_MESSAGE, "Message IHM cote texte\n");

        /*on le recoit*/
        if (itre_com_RecevoirMsg (vg_socRecFromAxMce,
            &vl_msg,
            ITRE_COM_MASQUE_TOUS,
            &vl_autreMsg) != XDC_OK)
        {
          /* Probleme socket : arreter la tache relais*/
          XZST_03EcritureTrace(XZSTC_WARNING, "%s : Probleme lecture canal IHM %s\n",
              vg_nomTache,
              vg_appli_est_mono_ecran ? "" : "texte");

          /* Fermer les canaux de communication et terminer */
          itre_dir_fin_communication();
          exit(4);
        }
        else
        {
          /* sinon traiter le message */
          if (vl_msg.codeMsg != ITRE_COM_MSG_PID_PROCESS)
            itre_dir_traiter_message_ihm(&vl_msg);
        }
      } while (vl_autreMsg == XDC_VRAI);
    }


    /* si message d'Applix cote IHM synoptique */
    if (!vg_appli_est_mono_ecran && FD_ISSET(vg_socRecFromAxDial, &vl_fdLire))
    {
      do {
        XZST_03EcritureTrace (XZSTC_MESSAGE, "Message IHM cote synoptique\n");

        /*on le recoit*/
        if (itre_com_RecevoirMsg (vg_socRecFromAxDial,
            &vl_msg,
            ITRE_COM_MASQUE_TOUS,
            &vl_autreMsg) != XDC_OK)
        {
          /* Probleme socket : arreter la tache relais*/
          XZST_03EcritureTrace(XZSTC_WARNING,
              "%s : Probleme lecture canal IHM synoptique\n",
              vg_nomTache);

          /* Fermer les canaux de communication et terminer */
          itre_dir_fin_communication();
          exit(4);
        }
        else
        {
          /* sinon traiter le message */
          itre_dir_traiter_message_ihm(&vl_msg);
        }
      } while (vl_autreMsg == XDC_VRAI);
    }
  }
}

int itre_lire_config_TOPS(XDY_Texte va_param, XDY_Texte va_IP, XDY_Entier *va_display, XDY_Entier *va_port)
{
	XDY_Texte vl_NomFichier;
	XDY_Texte pl_param;
	FILE* pl_fp;
	XDY_Texte pl_ligne;
	XZSCT_NomMachine  pl_NomMachine = "\0";
	XDY_Texte vl_IP;
	XDY_Entier vl_display,vl_port;
		
	XZSC_07NomMachine(pl_NomMachine);
	strcpy (vl_NomFichier,CO_FIC_TOPS_GEN_PATH);
	strcat(vl_NomFichier,pl_NomMachine);
	strcat(vl_NomFichier,".txt");
	
	/* Ouverture du fichier de config */
	if ( (pl_fp = fopen(vl_NomFichier, "r")) == NULL ) {
			XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
									,vl_NomFichier );
			return(XDC_NOK);
	}

	/* On lit les lignes du fichier une par une */
	/* ------------------------------------------------------ */

	while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
			/* On ignore les lignes commencant par un # */
			if ( pl_ligne[0] == '#' )
					continue;

			/* Pour les autres lignes on recherche la frequence de notre tache */
			if (sscanf(pl_ligne,"%s\t%s\t%d\t%d", pl_param, &vl_IP,&vl_display,&vl_port)==4)
			{
				if ( !strcmp(pl_param,va_param) ) 
				{
					strcpy(va_IP,vl_IP);
					*va_display=vl_display;
					*va_port=vl_port;
					XZST_03EcritureTrace(XZSTC_WARNING, "lecture fichier config TOP param fichier %s, ligne %s = %s:%d.%d",vl_NomFichier,pl_param,va_IP,*va_display,*va_port);
					fclose(pl_fp);
					return (XDC_OK);
				}				
			}
			else 
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "le fichier %s n'a pas le format demande !",vl_NomFichier );
			}
	}
	fclose(pl_fp);
	return (XDC_NOK);
}							

