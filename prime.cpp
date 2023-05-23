

int isPrime(unsigned int number) {
    // Check if number is 2 (the only even prime number)
    if (number == 2) {
        return 1;
    }

    // Check if number is less than 2 or even
    if (number < 2 || number % 2 == 0) {
        return 0;
    }

    // Check for divisibility by odd numbers up to the square root of the number
    for (unsigned int i = 3; i * i <= number; i += 2) {
        if (number % i == 0) {
            return 0;
        }
    }

    return 1;
}

