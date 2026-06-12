#!/usr/bin/env python3
import subprocess
import sys

# Couleurs pour un affichage propre dans le terminal
GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
BLUE = "\033[94m"
RESET = "\033[0m"
BOLD = "\033[1m"

BINARY = "./codexion"

# Base d'arguments valides qui s'arrêtent instantanément (1 seule compile requise)
# format: number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
VALID_BASE = ["2", "1000", "10", "10", "10", "1", "0", "fifo"]

class TestCase:
    def __init__(self, name, args, expect_fail=True, expected_msg=None):
        self.name = name
        self.args = args
        self.expect_fail = expect_fail
        self.expected_msg = expected_msg

tests = [
    # =========================================================================
    # 1. TESTS SUR LE NOMBRE D'ARGUMENTS
    # =========================================================================
    TestCase("Aucun argument", []),
    TestCase("Trop peu d'arguments (3 args)", ["2", "1000", "10"]),
    TestCase("Trop peu d'arguments (7 args)", ["2", "1000", "10", "10", "10", "1", "0"]),
    TestCase("Trop d'arguments (9 args)", VALID_BASE + ["extra_arg"], expect_fail=False), # Souvent ignoré par le parser de base

    # =========================================================================
    # 2. TESTS SUR LES CARACTÈRES INTERDITS (STRINGS / CHARS)
    # =========================================================================
    TestCase("Lettre dans number_of_coders", ["abc", "1000", "10", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),
    TestCase("Lettre au milieu d'un nombre", ["2", "10a0", "10", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),
    TestCase("Caractère spécial dans time_to_compile", ["2", "1000", "10!", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),
    TestCase("Argument vide (string vide)", ["2", "1000", "", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),
    TestCase("Espaces dans l'argument", ["2", "1000", "10 0", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),

    # =========================================================================
    # 3. TESTS SUR LES NOMBRES NÉGATIFS
    # =========================================================================
    TestCase("Nombre de codeurs négatif", ["-5", "1000", "10", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),
    TestCase("Time to burnout négatif", ["2", "-1000", "10", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),
    TestCase("Dongle cooldown négatif", ["2", "1000", "10", "10", "10", "1", "-50", "fifo"], expected_msg="negative value or str is forbiden"),

    # =========================================================================
    # 4. TESTS SUR LES NOMBRES À VIRGULE (FLOATS)
    # =========================================================================
    TestCase("Nombre à virgule (point)", ["2.5", "1000", "10", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),
    TestCase("Nombre à virgule (virgule)", ["2", "1000", "10,5", "10", "10", "1", "0", "fifo"], expected_msg="negative value or str is forbiden"),

    # =========================================================================
    # 5. TESTS SUR LES OVERFLOWS (LIMITES INT_MAX)
    # =========================================================================
    TestCase("Overflow gigantesque", ["9999999999999999999999999999", "1000", "10", "10", "10", "1", "0", "fifo"]),
    TestCase("INT_MAX + 1 (Alerte overflow)", ["2147483648", "1000", "10", "10", "10", "1", "0", "fifo"]),

    # =========================================================================
    # 6. TESTS SUR LE SCHEDULER
    # =========================================================================
    TestCase("Scheduler invalide (inconnu)", ["2", "1000", "10", "10", "10", "1", "0", "random_sched"], expected_msg="arg is not fifo or edf"),
    TestCase("Scheduler en MAJUSCULES", ["2", "1000", "10", "10", "10", "1", "0", "FIFO"], expected_msg="arg is not fifo or edf"),
    TestCase("Scheduler edf avec espace", ["2", "1000", "10", "10", "10", "1", "0", "edf "], expected_msg="arg is not fifo or edf"),
    TestCase("Scheduler vide", ["2", "1000", "10", "10", "10", "1", "0", ""], expected_msg="arg is not fifo or edf"),

    # =========================================================================
    # 7. TEST VALIDE (POUR VÉRIFIER QUE LE SCRIPT RECONNAÎT LE SUCCÈS)
    # =========================================================================
    TestCase("Arguments 100% Valides", VALID_BASE, expect_fail=False)
]

def run_tests():
    passed = 0
    failed = 0
    crashes = 0

    print(f"{BOLD}{BLUE}=================================================={RESET}")
    print(f"{BOLD}{BLUE}       LANCEMENT DES TESTS DE PARSING             {RESET}")
    print(f"{BOLD}{BLUE}=================================================={RESET}\n")

    for i, t in enumerate(tests, 1):
        print(f"{BOLD}Test {i:02d} : {t.name}{RESET}")
        print(f"  Command: {BINARY} {' '.join(t.args)}")
        
        try:
            # Exécution du binaire avec une limite de 2 secondes au cas où il freeze
            res = subprocess.run(
                [BINARY] + t.args,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                timeout=2
            )
            
            output = res.stderr if res.stderr else res.stdout
            returncode = res.returncode

            # Analyse des crashs (Segmentation Fault / SIGSEGV = code de retour négatif ou 139)
            if returncode < 0 or returncode == 139:
                print(f"  {RED}❌ CRASH (Segmentation Fault / Bus Error){RESET}")
                crashes += 1
                failed += 1
                continue

            # Vérification du comportement attendu
            if t.expect_fail:
                if returncode != 0:
                    # Le binaire a bien échoué (Ce qu'on voulait !)
                    if t.expected_msg and t.expected_msg not in output:
                        print(f"  {YELLOW}⚠️  Échec détecté mais message de la Norme absent ou incorrect.{RESET}")
                        print(f"     Attendu: '{t.expected_msg}'")
                        print(f"     Obtenu:  '{output.strip()}'")
                        failed += 1
                    else:
                        print(f"  {GREEN}✅ Succès (Rejeté proprement avec code {returncode}){RESET}")
                        passed += 1
                else:
                    print(f"  {RED}❌ ÉCHEC : Le binaire a accepté ces arguments invalides !{RESET}")
                    failed += 1
            else:
                if returncode == 0:
                    print(f"  {GREEN}✅ Succès (Arguments valides acceptés){RESET}")
                    passed += 1
                else:
                    print(f"  {RED}❌ ÉCHEC : Les arguments valides ont été rejetés (code {returncode}){RESET}")
                    print(f"     Output: {output.strip()}")
                    failed += 1

        except subprocess.TimeoutExpired:
            print(f"  {RED}❌ TIMEOUT : Le binaire a freezé au lieu de s'arrêter.{RESET}")
            failed += 1
        except FileNotFoundError:
            print(f"  {RED}❌ Erreur : L'exécutable '{BINARY}' est introuvable. Fais 'make'.{RESET}")
            sys.exit(1)
        print("-" * 50)

    # Bilan global
    print(f"\n{BOLD}{BLUE}==================== BILAN ===================={RESET}")
    print(f"  Total de tests : {len(tests)}")
    print(f"  {GREEN}Tests Réussis  : {passed}{RESET}")
    print(f"  {RED}Tests Échoués  : {failed} (dont {crashes} crashs/segfaults){RESET}")
    print(f"{BOLD}{BLUE}==============================================={RESET}")
    
    if failed > 0:
        sys.exit(1)
    sys.exit(0)

if __name__ == "__main__":
    run_tests()