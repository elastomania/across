tomb[i].hossz = abs( tomb[i].v );
		if( tomb[i].hossz == 0 )
			hiba( "Nulla hosszusagu szakasz!" );
		tomb[i].egyseg = egys( tomb[i].v );