d � &e � THIS IS EXAMPLE OF USING @f � DS12C887 FROM BASIC Fg � Lm : rn BASE�53760:� BASE ADDRESS OF CHIP xs : �� � GET TIME FROM RTC �� : �� �BASE,4:H��(BASE�1) �� �BASE,2:MI��(BASE�1) �� �BASE,0:S��(BASE�1) �� : 	� � SHOW WHAT WE HAVE GOT 	� : 	� V�H:�1100:�V;":"; 4	� V�MI:�1100:�V;":"; E	V�S:�1100:�V K	: f		� SETUP CIA1 TOD CLOCK l	
: �	PM�0:V�H:�1100:�V�13�280 �	V�V�12:�1000:H�V �	� 56329,S �	� 56328,0 �	": �	#� THAT'S ALL FOLKS �	': �	�� 
�� CONVERT V TO BCD, RETURN IN V '
�V��(V�10)�16�(V�10��(V�10)) -
�� S
L� CONVERT V FROM BCD, RETURN IN V s
VV��(V�16)�10�(V�16��(V�16)) y
`�   