import subprocess

def run_command(cmd):
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    return result.stdout

# Commandes à exécuter
cmd_exe = ["./exe", "1", "200", "300"]
cmd_exe_opti = ["./exe_opti", "1", "200", "300"]

# Exécution des commandes et récupération de la sortie
output_exe = run_command(cmd_exe)
output_exe_opti = run_command(cmd_exe_opti)

# Affichage des sorties
print("Output de ./exe:")
print(output_exe)

print("Output de ./exe_opti:")
print(output_exe_opti)

# Comparaison des sorties
if output_exe == output_exe_opti:
    print("Les deux sorties sont identiques.")
else:
    print("Les sorties sont différentes.")
