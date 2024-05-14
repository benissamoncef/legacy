/*E*/
/*Fichier :  @(#)xzao0203sp.prc	1.1      Release : 1.1        Date : 02/13/08
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao0203sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* HISTORIQUE :
*
* ABE   28/03/13	: gestion du retour à la normale avec ajout de scénarios pour le domaine exploitation
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao0203sp.h" */

/* static char *version = "@(#)xzao0203.c    1.1 23/18/94 : xzao0203" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Description de la GTC Nice pour TEINI
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go
drop procedure XZAO0203
go
create procedure XZAO0203
                 @va_Basedd_in              char(3),
                 @va_NomMachine_in          char(10),
                 @va_Numero_out             smallint  = null  output,
                 @va_NomServeur_out         char(10)  = null  output
       as

/* 
*
* SP    xzao0203SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier     va_Basedd_in        
* XDY_Machine    va_NomMachine_in        
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des GTC Nice Renovee
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    09 Nov 2007    : Creation
------------------------------------------------------*/
declare
   
    /*A
    ** Declaration des variables locales
    */
   
    @vl_Machine        smallint
    
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
    */
      
    if @va_Basedd_in = null
    or @va_NomMachine_in = null
    
        return XDC_ARG_INV
   
    else   
   
    /*A
    **   Les parametres d'entree ne sont pas nuls  
    */

    begin
      
        /*A
        **  Travail dans la base CFG
        */   
      
        if @va_Basedd_in = XDC_BASE_CFG
        begin   

            /*A
            ** Recherche du numero de la machine
            */
         
            select @vl_Machine = numero
            from   CFG..EQT_GEN 
            where  type = XDC_EQT_MAC
              and  nom  = @va_NomMachine_in
                 
            /*A
            ** Affectation des parametres de sorties (XZAOT_GTC_NICE_RENOVEE)
            */
         
            select  @va_Numero_out             = NIC.numero, 
                    @va_NomServeur_out         = GNIC.nom

            from    CFG..EQT_GEN GNIC,
                    CFG..EQT_NIC NIC
            where   GNIC.type        = XDC_EQT_NIC
              and   GNIC.maitre      = @vl_Machine
              and   GNIC.type_maitre = XDC_MAI_NIC
              and   NIC.numero       = GNIC.numero
         
         
            if @@rowcount != 0
            begin       

         
                /*A
                ** Liste des GTC des ouvrages
                */
                select   'O',
                         gen.nom
                from     CFG..EQT_GEN gen
                where    gen.type_maitre = XDC_EQT_NIC
                  and    gen.maitre = 1
                order by gen.nom
         
                /*A
                ** Liste des GTC du domaine signalisation
                */
                select 'S',
                       gen.type,
                       gen.numero,
                       0
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_SIG sig
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  sig.numero = gen.numero
                  and  sig.type = gen.type
                order by 1,2,3
            
                /*A
                ** Liste des GTC du domaine eclairage
                */
                select 'C',
                       gen.type,
                       gen.numero,
                       0
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_ECL ecl
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  ecl.numero = gen.numero
                  and  ecl.type = gen.type
                order by 1,2,3

                /*A
                ** Liste des GTC du domaine ventilation
                */
                select 'V',
                       gen.type,
                       gen.numero,
                       0
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_VEN ven
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  ven.numero = gen.numero
                  and  ven.type = gen.type
                order by 1,2,3

                /*A
                ** Liste des GTC du domaine energie
                */
                select 'N',
                       gen.type,
                       gen.numero,
                       0
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_ENR enr
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  enr.numero = gen.numero
                  and  enr.type = gen.type
                order by 1,2,3
                    
                /*A
                ** Liste des GTC du domaine protection incendie
                */
                select 'I',
                       gen.type,
                       gen.numero,
                       0
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_INC inc
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  inc.numero = gen.numero
                  and  inc.type = gen.type
                order by 1,2,3

                /*A
                ** Liste des GTC du domaine protection exploitation
                */
                select 'X',
                       gen.type,
                       gen.numero,
                       0
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_EXP exp
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  exp.numero = gen.numero
                  and  exp.type = gen.type
                order by 1,2,3
    
                /*A
                ** Liste des GTC du domaine protection systeme
                */
                select 'Y',
                       gen.type,
                       gen.numero,
                       0
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_SYS sys
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  sys.numero = gen.numero
                  and  sys.type = gen.type
                order by 1,2,3
    
                /*A
                ** Liste des GTC du domaine protection locaux techniques
                */
                select 'L',
                       gen.type,
                       gen.numero,
                       0
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_LTN ltn
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  ltn.numero = gen.numero
                  and  ltn.type = gen.type
                order by 1,2,3

/* AZAZA Liste des equipements a faire*/

                /*A
                ** Liste des equipements du domaine signalisation
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'S'
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_SIG sig
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  sig.numero = gen.numero
                  and  sig.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine eclairage
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'C'
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_ECL ecl
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  ecl.numero = gen.numero
                  and  ecl.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine ventilation
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'V'
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_VEN ven
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  ven.numero = gen.numero
                  and  ven.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine energie
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'N'
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_ENR enr
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  enr.numero = gen.numero
                  and  enr.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine protection incendie
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'I'
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_INC inc
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  inc.numero = gen.numero
                  and  inc.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine exploitation
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'X'
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_EXP exp
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  exp.numero = gen.numero
                  and  exp.type = gen.type
                order by 2, 3, 4


                /*A
                ** Liste des equipements du domaine systeme
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'Y'
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_SYS sys
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  sys.numero = gen.numero
                  and  sys.type = gen.type
                order by 2, 3, 4


                /*A
                ** Liste des equipements du domaine locaux techniques
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'L'
                from   CFG..EQT_GEN gen,
                       CFG..EQT_GTC_LTN ltn
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  ltn.numero = gen.numero
                  and  ltn.type = gen.type
                order by 2, 3, 4


/* AZAZAZAZA */

                /*A
                ** Liste des GTC Sequence du domaine signalisation
                */
                select   'S',
                         type, 
                         equipement,
                         sequence
                from     CFG..EQT_SQA_GTC_SIG
                order by sequence,
                         type,
                         equipement
    
                /*A
                ** Liste des GTC Sequence du domaine eclairage
                */
                select   'C',
                         type,
                         equipement,
                         sequence
                from     CFG..EQT_SQA_GTC_ECL
                order by sequence,
                         type,
                         equipement

                /*A
                ** Liste des GTC Sequence du domaine exploitation
                */
                select   'X',
                         type,
                         equipement,
                         sequence
                from     CFG..EQT_SQA_GTC_EXP
                order by sequence,
                         type,
                         equipement
                             
                /*A
                ** Liste des GTC Sequence du domaine ventilation
                */
                select   'V',
                         type,
                         equipement,
                         sequence
                from     CFG..EQT_SQA_GTC_VEN
                order by sequence,
                         type,
                         equipement
    
                /*A
                ** Liste des GTC Sequence du domaine energie
                */
                select   'N',
                         type,
                         equipement,
                         sequence
                from     CFG..EQT_SQA_GTC_ENR
                order by sequence,
                         type,
                         equipement

                /*A
                ** Liste des GTC Sequence du domaine protection incendie
                */
                select   'I',
                         type,
                         equipement,
                         sequence
                from     CFG..EQT_SQA_GTC_INC
                order by sequence,
                         type,
                         equipement




            end
     
        end
      
#ifdef CI
      
        else
      
        /*A
        **  Travail dans la base CFT
        */   
      
        begin   


            /*A
            ** Recherche du numero de la machine
            */
         
            select @vl_Machine = numero
            from   CFT..EQT_GEN 
            where  type = XDC_EQT_MAC
              and  nom  = @va_NomMachine_in
                 
            /*A
            ** Affectation des parametres de sorties (XZAOT_GTC_NICE_RENOVEE)
            */
         
            select  @va_Numero_out             = NIC.numero, 
                    @va_NomServeur_out         = GNIC.nom

            from    CFT..EQT_GEN GNIC,
                    CFT..EQT_NIC NIC
            where   GNIC.type        = XDC_EQT_NIC
              and   GNIC.maitre      = @vl_Machine
              and   GNIC.type_maitre = XDC_MAI_NIC
              and   NIC.numero       = GNIC.numero
         
         
            if @@rowcount != 0
            begin       

         
                /*A
                ** Liste des GTC des ouvrages
                */
                select   'O',
                         gen.nom
                from     CFT..EQT_GEN gen
                where    gen.type_maitre = XDC_EQT_NIC
                  and    gen.maitre = 1
                order by gen.nom
         
                /*A
                ** Liste des GTC du domaine signalisation
                */
                select 'S',
                       gen.type,
                       gen.numero,
                       0
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_SIG sig
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  sig.numero = gen.numero
                  and  sig.type = gen.type
                order by 1,2,3
            
                /*A
                ** Liste des GTC du domaine eclairage
                */
                select 'C',
                       gen.type,
                       gen.numero,
                       0
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_ECL ecl
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  ecl.numero = gen.numero
                  and  ecl.type = gen.type
                order by 1,2,3

                /*A
                ** Liste des GTC du domaine ventilation
                */
                select 'V',
                       gen.type,
                       gen.numero,
                       0
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_VEN ven
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  ven.numero = gen.numero
                  and  ven.type = gen.type
                order by 1,2,3

                /*A
                ** Liste des GTC du domaine energie
                */
                select 'N',
                       gen.type,
                       gen.numero,
                       0
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_ENR enr
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  enr.numero = gen.numero
                  and  enr.type = gen.type
                order by 1,2,3
                    
                /*A
                ** Liste des GTC du domaine protection incendie
                */
                select 'I',
                       gen.type,
                       gen.numero,
                       0
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_INC inc
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  inc.numero = gen.numero
                  and  inc.type = gen.type
                order by 1,2,3

                /*A
                ** Liste des GTC du domaine protection exploitation
                */
                select 'X',
                       gen.type,
                       gen.numero,
                       0
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_EXP exp
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  exp.numero = gen.numero
                  and  exp.type = gen.type
                order by 1,2,3
    
                /*A
                ** Liste des GTC du domaine protection systeme
                */
                select 'Y',
                       gen.type,
                       gen.numero,
                       0
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_SYS sys
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  sys.numero = gen.numero
                  and  sys.type = gen.type
                order by 1,2,3
    
                /*A
                ** Liste des GTC du domaine protection locaux techniques
                */
                select 'L',
                       gen.type,
                       gen.numero,
                       0
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_LTN ltn
                where  gen.type_maitre = XDC_EQT_NIC
                  and  gen.maitre = 1
                  and  ltn.numero = gen.numero
                  and  ltn.type = gen.type
                order by 1,2,3

/* AZAZA Liste des equipements a faire*/

                /*A
                ** Liste des equipements du domaine signalisation
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'S'
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_SIG sig
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  sig.numero = gen.numero
                  and  sig.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine eclairage
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'C'
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_ECL ecl
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  ecl.numero = gen.numero
                  and  ecl.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine ventilation
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'V'
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_VEN ven
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  ven.numero = gen.numero
                  and  ven.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine energie
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'N'
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_ENR enr
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  enr.numero = gen.numero
                  and  enr.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine protection incendie
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'I'
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_INC inc
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  inc.numero = gen.numero
                  and  inc.type = gen.type
                order by 2, 3, 4

                /*A
                ** Liste des equipements du domaine exploitation
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'X'
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_EXP exp
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  exp.numero = gen.numero
                  and  exp.type = gen.type
                order by 2, 3, 4


                /*A
                ** Liste des equipements du domaine systeme
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'Y'
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_SYS sys
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  sys.numero = gen.numero
                  and  sys.type = gen.type
                order by 2, 3, 4


                /*A
                ** Liste des equipements du domaine locaux techniques
                */
                select 'Q',
                       gen.type_maitre,
                       gen.maitre,
                       gen.type,
                       gen.numero,
                       'L'
                from   CFT..EQT_GEN gen,
                       CFT..EQT_GTC_LTN ltn
                where  gen.type_maitre <> XDC_EQT_NIC
                  and  ltn.numero = gen.numero
                  and  ltn.type = gen.type
                order by 2, 3, 4


/* AZAZAZAZA */

                /*A
                ** Liste des GTC Sequence du domaine signalisation
                */
                select   'S',
                         type, 
                         equipement,
                         sequence
                from     CFT..EQT_SQA_GTC_SIG
                order by sequence,
                         type,
                         equipement
    
                /*A
                ** Liste des GTC Sequence du domaine eclairage
                */
                select   'C',
                         type,
                         equipement,
                         sequence
                from     CFT..EQT_SQA_GTC_ECL
                order by sequence,
                         type,
                         equipement

                /*A
                ** Liste des GTC Sequence du domaine exploitation
                */
                select   'X',
                         type,
                         equipement,
                         sequence
                from     CFT..EQT_SQA_GTC_EXP
                order by sequence,
                         type,
                         equipement

                /*A
                ** Liste des GTC Sequence du domaine ventilation
                */
                select   'V'
                         type,
                         equipement,
                         sequence
                from     CFT..EQT_SQA_GTC_VEN
                order by sequence,
                         type,
                         equipement
    
                /*A
                ** Liste des GTC Sequence du domaine energie
                */
                select   'N'
                         type,
                         equipement,
                         sequence
                from     CFT..EQT_SQA_GTC_ENR
                order by sequence,
                         type,
                         equipement

                /*A
                ** Liste des GTC Sequence du domaine protection incendie
                */
                select   'I'
                         type,
                         equipement,
                         sequence
                from     CFT..EQT_SQA_GTC_INC
                order by sequence,
                         type,
                         equipement




            end
         
        end
#endif
      
    end
      
    return XDC_OK
         
go  
