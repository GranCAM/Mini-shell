# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: carbon <carbon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/25 13:18:53 by guigonza          #+#    #+#              #
#    Updated: 2025/07/12 19:26:39 by carbon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			=	ft_bzero.c \
					ft_isalnum.c \
					ft_isalpha.c \
					ft_isascii.c \
					ft_isdigit.c \
					ft_isprint.c \
					ft_memcmp.c \
					ft_memcpy.c \
					ft_memset.c \
					ft_memmove.c \
					ft_memchr.c \
					ft_strlen.c \
					ft_strnstr.c \
					ft_strchr.c \
					ft_strncmp.c \
					ft_strrchr.c \
					ft_strlcat.c \
					ft_strlcpy.c \
					ft_tolower.c \
					ft_toupper.c \
					ft_putchar_fd.c \
					ft_putendl_fd.c \
					ft_putstr_fd.c \
					ft_putendl_fd.c \
					ft_calloc.c \
					ft_strdup.c \
					ft_strjoin.c \
					ft_substr.c \
					ft_strtrim.c \
					ft_putnbr_fd.c \
					ft_itoa.c ft_atoi.c\
					ft_striteri.c \
					ft_isspace.c \
					ft_strmapi.c \
					ft_split.c\
					ft_realloc.c \
					ft_free.c\
					ft_strjoin_free.c\


OBJS			= $(SRCS:.c=.o)

BONUS			=	ft_lstnew_bonus.c  ft_lstadd_front_bonus.c \
					ft_lstlast_bonus.c ft_lstsize_bonus.c \
					ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
					ft_lstclear_bonus.c	ft_lstiter_bonus.c \
					ft_lstmap_bonus.c 
OBJS_B			= $(BONUS:.c=.o)
CC				= cc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -g3 -I.

NAME			= libft.a

all:			$(NAME)

$(NAME):		$(OBJS)
				ar rcs $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS) $(OBJS_B)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

bonus:			$(NAME) $(OBJS_B) 
				ar rcs $(NAME) $(OBJS_B) 

.PHONY:			all clean fclean re bonus
