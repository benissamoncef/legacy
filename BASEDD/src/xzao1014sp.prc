/*E*/
/*Fichier :  $Id: xzao1014sp.prc,v 1.5 2018/07/11 11:14:39 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 11:14:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1014sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/
/* fichiers inclus */

/* #include "xzao1014sp.h" */

/* static char *version = "@(#)xzao1014.c    1.1 11/15/07 : xzao1014" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Ajout d'une sequence autorisee ddu domaine EXP dans la base
*
* HISTORIQUE :
*
* ABE 06-05-13 : gestion du retour à la normale avec ajout de scénarios pour le domaine exploitation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go
drop procedure XZAO1014
go
  create procedure XZAO1014
    @va_Basedd_in           char(3),
    @va_NomSequence_in      T_NOM,
    @va_Type_in             tinyint,
    @va_NomEXP_in           T_NOM,
    @va_Creation_in         bit,
    @va_Resultat_out        int output
    as

/* 
* 
 
* SP    XZAO1014SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_Nom           va_NomSequence_in
* XDY_Octet         va_Type_in
* XDY_Nom           va_NomEXP_in
* XDY_Booleen       va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_R 
 esConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une sequence autorisee d'un tunnel en base
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification d 
 ans la table EQT_SQA_GTC_EXP (EQUIPEMENT_SQA_EXP)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    15 Nov 2007    : Creation
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare

    /*A
    * 
 * Definition des variables globales
    */

    @vl_NumEXP      smallint,
    @vl_NumE        smallint,
    @vl_Type        tinyint,
    @vl_NumSeq      tinyint,
    @vl_NumS        tinyint,
	@vl_Site		char(10)
	
    select @va_Resultat_out = 4 

    /*A
    ** Test des parame 
 tres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */

    if @va_Basedd_in         = null
    or @va_NomSequence_in    = null
    or @va_Type_in           = null
    or @va_NomEXP_in         = null
    or @va_Creation_in       = null

        return 2 

    else
    begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin

            /*A
			  **  Travail dans la base CFG
		           */
 
             if @va_Basedd_in = "CFG" 
             begin
                /*A
			             ** Verification de l'existence de la sequence dans EQT_SEQ_GTC_EXP
				*/
                select @vl_NumSeq = numero
                from   CFG..EQT_SEQ_GTC_EXP
   
                where  nom = @va_NomSequence_in

                /*A
                ** La sequence existe dans EQT_SEQ_GTC_EXP
                */

                if @vl_NumSeq <> null
                begin

                    select @vl_NumEXP = numero
  
                    from   CFG..EQT_GEN
                    where  nom  = @va_NomEXP_in
                      and  type = @va_Type_in
                    /*A
                    ** L equipement du domaine EXP existe dans EQT_GEN
                    */

    
                    if @vl_NumEXP <> null
                    begin

                        select @vl_NumE   = equipement, @vl_NumS = sequence
                        from   CFG..EQT_SQA_GTC_EXP
                        where  type       = @va_Type_in
                        and  equipement = @vl_NumEXP
                          and  sequence   = @vl_NumSeq

                        /*A
                        ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_GTC_EXP
                        */

 
                         if @vl_NumE = null and @vl_NumS = null
                         begin

                            /*A
                            ** Creation positionne =>
                            ** Insertion de la sequence autorisee d'un tunn 
 el dans la table EQT_SQA_TUB
                            */

                            if @va_Creation_in = 1 
                            begin

                                insert into CFG..EQT_SQA_GTC_EXP ( type,equipement,sequence )
                                                          values ( @va_Type_in,@vl_NumEXP,    @vl_NumSeq )

                                /*A
                                ** Si erreur d'insertion
                                */

                   
               			if @@rowcount <> 1
                                begin

                                   /*A
                                   ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                                   */
				   select @va_Resultat_out = 1 

                                   return 1 
                                end

                                /*A
                                ** Affectation du parametre de sortie Resultat a XD 
 C_AJOUT
                                */

                                select @va_Resultat_out = 1 

                             end

                             else

                                 /*A
                                 ** Creatio 
 n non positionne =>
                                 ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                                 */

                                select @va_Resultat_out = 4 

                          end

              
              else

                          /*A
                          ** La sequence autorisee n'existe pas dans EQT_SQA_GTC_EXP
                          */

                          begin
							 select @va_Resultat_out = 1 

                      
         if @va_Creation_in <> 1 

                             /*A 
                             ** Creation non positionne =>
                             ** Suppression de la sequence autorisee dand la table EQT_SQA_GTC_EXP
                              
 */

                            begin

                                delete CFG..EQT_SQA_GTC_EXP
                                where  type       = @va_Type_in
                                  and  equipement = @vl_NumEXP
                              
      and  sequence   = @vl_NumSeq

                                /*A
                                ** Si erreur de suppression
                                */

                                if @@rowcount <> 1
                                begin 
 

                                   /*A
                                   ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                   */

                                   select @va_Resultat_out = 3 

                 
                    return 1 
                                end

                                /*A
                                ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                */

                           
       select @va_Resultat_out = 3 
                            end
                        end
                    end

                    else

                        /*A
                        ** L'equipement n'existe pas 
                        **  
 Affectation du parametre de sortie Resultat a XDC_ECHEC
                        */

                        select @va_Resultat_out = 4 
                end

                else

                    /*A
                    ** La sequence du tunnel n'exis 
 te pas 
                    ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                    */

                    select @va_Resultat_out = 1 

            end


            else

            /*A
            **  Travail dans la base CFT
  
            */

            begin


                /*A
                ** Verification de l'existence de la sequence dans EQT_SEQ_GTC_EXP
                */
		
                select @vl_NumSeq = numero
                from   CFT..EQT_SEQ_GTC_EXP
           
       		where  nom = @va_NomSequence_in

                /*A
                ** La sequence existe dans EQT_SEQ_GTC_EXP
                */

                if @vl_NumSeq <> null
                begin

                    select @vl_NumEXP = numero
          
            		from   CFT..EQT_GEN
                    where  nom  = @va_NomEXP_in
                      and  type = @va_Type_in
                    /*A
                    ** L equipement du domaine EXP existe dans EQT_GEN
                    */

            
          	    if @vl_NumEXP <> null
                    begin

                        select @vl_NumE   = equipement, @vl_NumS = sequence
                        from   CFT..EQT_SQA_GTC_EXP
                        where  type       = @va_Type_in
              
              and  equipement = @vl_NumEXP
                          and  sequence   = @vl_NumSeq

                        /*A
                        ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_GTC_EXP
                        */

         
                 	if @vl_NumE = null and @vl_NumS = null
                        begin

                            /*A
                            ** Creation positionne =>
                            ** Insertion de la sequence autorisee d'un tunnel dans  
 la table EQT_SQA_TUB
                            */

                            if @va_Creation_in = 1 
                            begin

                                insert into CFT..EQT_SQA_GTC_EXP ( type, equipement,   sequence )
                                                          values ( @va_Type_in, @vl_NumEXP,   @vl_NumSeq )
                                /*A
                                ** Si erreur d'insertion
                                */
       				if @@rowcount <> 1
                                begin
                                   /*A
                                   ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                                   */

                 		    select @va_Resultat_out = 1 

                                   return 1 
                                end

                                /*A
                                ** Affectation du parametre de sortie Resultat a XDC_AJOUT
 
                                 */

                                select @va_Resultat_out = 1 

                             end

                             else
				begin
                                 /*A
                                 ** Creation non po 
 sitionne =>
                                 ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                                 */

                                select @va_Resultat_out = 4 
				
				end
                          end

                      
      else
                          /*A
                          ** La sequence autorisee n'existe pas dans EQT_SQA_GTC_EXP
                          */

                          begin

			select @va_Resultat_out = 1 


                              
 				if @va_Creation_in <> 1 

                             /*A 
                             ** Creation non positionne =>
                             ** Suppression de la sequence autorisee dand la table EQT_SQA_GTC_EXP
                             */

     
                         	begin

                                delete CFT..EQT_SQA_GTC_EXP
                                where  type       = @va_Type_in
                                  and  equipement = @vl_NumEXP
                                  and 
   sequence   = @vl_NumSeq

                                /*A
                                ** Si erreur de suppression
                                */

                                if @@rowcount <> 1
                                begin

       
                              /*A
                                   ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                   */

                                   select @va_Resultat_out = 3 
			            return 1 
                                end

                                /*A
                                ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                */

                                select @va_Resultat_out = 3 
                            	end

					
                        end
 
                   end

                    else
                        /*A								
                        ** L'equipement n'existe pas 
                        ** Affectat 
 ion du parametre de sortie Resultat a XDC_ECHEC
                        */

                        select @va_Resultat_out = 4 
                end

                else

                    /*A
                    ** La sequence du tunnel n'existe pas 
 
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                    */

                    select @va_Resultat_out = 1 

            end


        end
    end

    return 0 
go
