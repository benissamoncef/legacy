/*E*/
/*Fichier :  $Id: xzao226sp.prc,v 1.4 2018/07/11 10:56:27 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 10:56:27 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao226sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  < Module Objets Statiques >
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	20/06/07	: Creation (DEM 657) 1.1
* LCL	11/07/18	: correction syntaxe from
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Ajout d'un Canton dans la base
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO226_Ajouter_Canton
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_debut_in
* XDY_Sens	va_Sens_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	va_Resultat_out
*
* VALEUR RENDUE :
*
*  Ajoute un canton en base
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK     : erreur lors de l'insertion
* XDC_ARG_INV : parametre d'entree obligatoire Null
* <0          : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
*  Config OFF LINE
*
* FONCTION :
*
*  Insertion ou modification dans la table RES_CAN (CANTON)
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO226' and type = 'P')
	drop procedure XZAO226
go


create procedure XZAO226
	@va_Basedd_in		char(3),
	@va_Autoroute_in	T_AUTOROUTE,
	@va_PR_debut_in		T_PR,
	@va_Sens_in		T_SENS,
	@va_Resultat_out	int		output
as

declare
	@vl_Autoroute	T_AUTOROUTE,
	@vl_NumPortion	smallint,
	@vl_NumCanton	smallint,
	@vl_Site		char(10)


	/*A Controle des parametres d'entree obligatoires */
	if @va_Basedd_in = null
	or @va_Autoroute_in = null
	or @va_PR_debut_in = null
	or @va_Sens_in = null
		return XDC_ARG_INV


	select @va_Resultat_out = XDC_LU

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin

         /*A
         **  Travail dans la base CFG
         */

         if @va_Basedd_in = XDC_BASE_CFG
         begin

            /*A
            ** Verification de l'existence et recherche de l'autoroute et de la portion
            */

            select @vl_NumPortion = numero from CFG..RES_POR
              where autoroute in
                (select numero from CFG..RES_AUT
                   where nom       = @va_Autoroute_in)
              and PR_debut <  @va_PR_debut_in
              and PR_fin   >= @va_PR_debut_in

            /*A
            ** Existence de l'autoroute et de la portion
            */

            if @vl_NumPortion <> null
            begin

               select @vl_NumCanton = portion from CFG..RES_CAN
                 where PR_debut     = @va_PR_debut_in
                   and sens         = @va_Sens_in

               /*A
               ** Le Canton n'existe pas dans RES_CAN
               */

               if @vl_NumCanton = null
               begin

                  /*A
                  ** Insertion du Canton dans la table RES_CAN
                  */

                  insert into CFG..RES_CAN
                  values ( @vl_NumPortion, @va_PR_debut_in, @va_Sens_in )

                  /*A
                  ** Erreur d'insertion
                  */

                  if @@rowcount <> 1
                  begin

                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */

                     select @va_Resultat_out = XDC_AJOUT

                     return XDC_NOK
                  end

                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */

                  select @va_Resultat_out = XDC_AJOUT

               end
               else
               begin
                  update CFG..RES_CAN
                  set portion = @vl_NumPortion
                  where PR_debut = @va_PR_debut_in
                    and sens = @va_Sens_in
               end

            end

            else

            /*A
            ** L'autoroute ou la portion n'existe pas
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */

               select @va_Resultat_out = XDC_ECHEC
         end

#ifdef CI

         else

         /*A
         **  Travail dans la base CFT
         */

         begin

            /*A
            ** Verification de l'existence et recherche de l'autoroute et de la portion
            */

            select @vl_NumPortion = numero from CFT..RES_POR
              where autoroute in
                (select numero from CFT..RES_AUT
                   where nom       = @va_Autoroute_in)
              and PR_debut <  @va_PR_debut_in
              and PR_fin   >= @va_PR_debut_in

            /*A
            ** Existence de l'autoroute et de la portion
            */

            if @vl_NumPortion <> null
            begin

               select @vl_NumCanton = portion from CFT..RES_CAN
               where PR_debut       = @va_PR_debut_in
                 and sens           = @va_Sens_in

               /*A
               ** Le Canton n'existe pas dans RES_CAN
               */

               if @vl_NumCanton = null
               begin

                  /*A
                  ** Insertion du Canton dans la table RES_CAN
                  */

                  insert into CFT..RES_CAN
                  values ( @vl_NumPortion, @va_PR_debut_in, @va_Sens_in )

                  /*A
                  ** Erreur d'insertion
                  */

                  if @@rowcount <> 1
                  begin

                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */

                     select @va_Resultat_out = XDC_AJOUT

                     return XDC_NOK
                  end

                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */

                  select @va_Resultat_out = XDC_AJOUT

               end
               else
               begin
                  update CFT..RES_CAN
                  set portion = @vl_NumPortion
                  where PR_debut = @va_PR_debut_in
                    and sens = @va_Sens_in
               end

            end

            else

            /*A
            ** L'autoroute ou la portion n'existe pas
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */

               select @va_Resultat_out = XDC_ECHEC
         end

#endif

      end

   return XDC_OK
go
