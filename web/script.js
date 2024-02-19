function inviaModulo() {
    var nome = document.getElementById('nome').value;
    var email = document.getElementById('email').value;

    if (nome === "" || email === "") {
        alert("Si prega di compilare tutti i campi.");
        return;
    }

    var url = "http://localhost:8002";

    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            nome: nome,
            email: email
        }),
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Errore nella richiesta al server');
        }
        return response.json();
    })
    .then(data => {
        console.log("Dati inviati con successo al server.", data);
        document.getElementById('mioForm').reset();
    })
    .catch(error => {
        console.error('Si è verificato un errore:', error);
    });
}