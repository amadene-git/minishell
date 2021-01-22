gcc -I ~/Documents/42-cursus/Libft built_in_echo.c -g ~/Documents/42-cursus/Libft/libft.a -o in_echo
echo "./in_echo $@" | bash > diff1
echo "echo $@" | bash > diff2 
diff diff1 diff2
