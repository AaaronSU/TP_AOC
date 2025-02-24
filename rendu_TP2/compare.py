import subprocess

def run_command(cmd):
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    return result.stdout

# Commandes à exécuter
cmd_exe = ["./exe", "1", "200", "300"]
cmd_exe_opti = ["./exe_opti_v1", "1", "200", "300"]
cmd_exe_opti_v2 = ["./exe_opti_v2", "1", "200", "300"]

# Exécution des commandes et récupération de la sortie
output_exe = run_command(cmd_exe)
output_exe_opti = run_command(cmd_exe_opti)
output_exe_opti_v2 = run_command(cmd_exe_opti_v2)

# Affichage des sorties
print("Output de ./exe:")
print(output_exe)

print("Output de ./exe_opti:")
print(output_exe_opti)

print("Output de ./exe_opti_2:")
print(output_exe_opti_v2)

# Comparaison des sorties
if output_exe == output_exe_opti == output_exe_opti_v2:
    print("Les deux sorties sont identiques.")
else:
    print("Les sorties sont différentes.")
