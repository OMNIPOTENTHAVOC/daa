import random

with open("numbers.csv", "w") as f:
    # Generate 500 numbers
    for _ in range(500):
        number = random.randint(1, 10000)
        f.write(f"{number}\n")

print("Successfully created 'numbers.csv' with 500 integers in a column.")