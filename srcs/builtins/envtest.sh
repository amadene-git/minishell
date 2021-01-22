gcc -I ~/Documents/42-cursus/Libft built_in_env.c -g ~/Documents/42-cursus/Libft/libft.a -o in_env
echo "./in_env $@" | bash > diff1
echo "env $@" | bash > diff2 
diff diff1 diff2
