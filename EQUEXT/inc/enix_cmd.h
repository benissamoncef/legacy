/*E*/
/*Fichier :  @(#)enix_cmd.h	1.1      Release : 1.1        Date : 02/13/08
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la
* tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.1 03 Sept 2007    : GTC rénovée DEM 664
* ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
*
------------------------------------------------------ */
 
#ifndef enix_cmd
#define enix_cmd

/* fichiers inclus */

#include        "enix_don.h"

/* definitions de constantes exportees */

extern XDY_NomMachine vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern int            vg_maitre_actif;


extern XDY_NomMachine vg_nom_machine_TWWI0;
extern XDY_NomMachine vg_nom_machine_TWWI1;

extern int            vg_Port_SockTWWI0;
extern int            vg_Port_SockTWWI1;

extern int      vg_TypeEqt_LiaiTWWI0 ;
extern int      vg_NumEqt_LiaiTWWI0 ;

extern int      vg_TypeEqt_LiaiTWWI1 ;
extern int      vg_NumEqt_LiaiTWWI1 ;

extern int            vg_etat_SockTWWI0;
extern int            vg_etat_SockTWWI1;


/* delaration de fonctions externes */
/* AAZ 07 et 03/09/2007 ajout SIG, ECL, VEN, ENR, INC */
                          
extern  void    et_Init_IHM_TNIC_cb (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

/* AAZ 03/09/2007 */
extern  void    et_cmd_sig_cb       (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_ecl_cb       (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_ven_cb       (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_enr_cb       (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );
										
extern  void    et_cmd_exp_cb       (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_inc_cb       (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_seq_sig_cb   (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_seq_ecl_cb   (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_seq_ven_cb   (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_seq_enr_cb   (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_seq_inc_cb   (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );

extern  void    et_cmd_seq_exp_cb   (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );
										
extern  void    et_cmd_domaine_cb   (   T_IPC_CONN,
                                        T_IPC_CONN_PROCESS_CB_DATA,
                                        T_CB_ARG );
/* AAZ */

extern  void    et_controle_cb      (   T_IPC_CONN                      va_Cnx,
                                        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
                                        T_CB_ARG                        pa_ARG);



#endif
