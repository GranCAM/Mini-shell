<div align="center">

# 🐚 minishell

**Un intérprete de comandos minimalista**

[![42 School](https://img.shields.io/badge/42-School-000000?style=for-the-badge&logo=42&logoColor=white)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Norminette](https://img.shields.io/badge/Norminette-Passing-brightgreen?style=for-the-badge)](https://github.com/42School/norminette)

---

**Desarrollado por [Guillermo González (guigonza)](https://github.com/guigonza) y [Carlos (carbon-m)](https://github.com/GranCAM)**

</div>

---

## 📋 Descripción

**minishell** es un intérprete de comandos desarrollado en C siguiendo las especificaciones del proyecto homónimo de 42 School. Reproduce el comportamiento básico de bash, implementando parsing de comandos, ejecución de procesos, redirecciones, pipes, heredoc y builtins esenciales.

El objetivo principal es comprender en profundidad cómo funcionan los shells Unix: gestión de procesos, comunicación entre ellos mediante pipes, manejo de descriptores de fichero y señales.

---

## ✨ Características

| Funcionalidad | Descripción |
|---------------|-------------|
| **Prompt interactivo** | Espera y lee comandos del usuario usando readline |
| **Historial** | Navegación por comandos anteriores |
| **Ejecución de binarios** | Búsqueda en PATH y ejecución con rutas absolutas/relativas |
| **Pipes** | Encadenamiento de comandos con `\|` |
| **Redirecciones** | Entrada `<`, salida `>`, append `>>`, heredoc `<<` |
| **Variables de entorno** | Expansión de `$VAR` y `$?` |
| **Comillas** | Manejo de comillas simples y dobles |
| **Señales** | Gestión de `ctrl-C`, `ctrl-D`, `ctrl-\` |

### Builtins implementados

| Comando | Descripción |
|---------|-------------|
| `echo` | Imprime argumentos (soporta `-n`) |
| `cd` | Cambia de directorio |
| `pwd` | Muestra el directorio actual |
| `export` | Define variables de entorno |
| `unset` | Elimina variables de entorno |
| `env` | Muestra el entorno |
| `exit` | Termina el shell |

---

## 🛠️ Compilación

### Requisitos

- Compilador C (cc / clang / gcc)
- make
- readline
  - **macOS**: `brew install readline`
  - **Linux**: `sudo apt install libreadline-dev`

### Instrucciones

```bash
# Clonar el repositorio
git clone https://github.com/guigonza/minishell.git
cd minishell

# Compilar
make

# Limpiar objetos
make clean

# Limpiar todo (objetos + binario)
make fclean

# Recompilar desde cero
make re
```

### Nota para macOS

Si el enlazado falla por readline, exporta las rutas de Homebrew:

```bash
export LDFLAGS="-L$(brew --prefix readline)/lib"
export CPPFLAGS="-I$(brew --prefix readline)/include"
make re
```

---

## 🚀 Uso

```bash
./minishell
```

### Ejemplos

```bash
# Comando simple
minishell$ ls -la

# Pipes
minishell$ cat archivo.txt | grep palabra | wc -l

# Redirecciones
minishell$ echo "Hola mundo" > salida.txt
minishell$ cat < entrada.txt >> salida.txt

# Heredoc
minishell$ cat << EOF
> línea 1
> línea 2
> EOF

# Variables de entorno
minishell$ echo $USER
minishell$ export MI_VAR="valor"
minishell$ echo $MI_VAR

# Código de salida
minishell$ ls archivo_inexistente
minishell$ echo $?
```

---

## 📁 Estructura del proyecto

```
minishell/
├── src/                    # Código fuente (.c)
│   ├── main.c              # Punto de entrada
│   ├── parser*.c           # Parsing y tokenización
│   ├── executor*.c         # Ejecución de comandos
│   ├── builtins*.c         # Comandos internos
│   ├── signals.c           # Manejo de señales
│   ├── heredoc*.c          # Implementación de heredoc
│   └── ...
├── includes/               # Cabeceras (.h)
│   └── minishell.h
├── libft/                  # Biblioteca auxiliar
├── obj/                    # Objetos compilados
├── Makefile
└── README.md
```

---

## 📚 Aprendizajes

- **Procesos Unix**: fork, execve, wait, pipes
- **Descriptores de fichero**: dup2, redirecciones
- **Señales**: sigaction, manejo de SIGINT/SIGQUIT
- **Parsing**: tokenización, máquinas de estado, expansión de variables
- **Gestión de memoria**: prevención de leaks, liberación correcta
- **Trabajo en equipo**: Git, code review, división de tareas

---

## 🧪 Testing

El proyecto ha sido probado con:
- Casos básicos de bash
- Edge cases de redirecciones y pipes
- Manejo de errores y señales
- Validación con norminette

---

## 👥 Autores

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/guigonza">
        <b>Guillermo González</b>
      </a>
      <br>
      <sub>guigonza</sub>
    </td>
    <td align="center">
      <a href="https://github.com/GranCAM">
        <b>Carlos</b>
      </a>
      <br>
      <sub>carbon-m</sub>
    </td>
  </tr>
</table>

---

## 📄 Licencia

Proyecto académico desarrollado como parte del currículum de [42 School](https://42.fr).

---

<div align="center">

**⭐ Si te ha sido útil, considera dar una estrella al repositorio ⭐**

</div>
