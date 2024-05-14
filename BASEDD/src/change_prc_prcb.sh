					for filepath in *
					do
					  if test -f "$filepath"
					    then
					        sed 's/use PRC/use PRCB/g' "$filepath" > ../src.develop.prcb/"$filepath"
						  fi
						  done
