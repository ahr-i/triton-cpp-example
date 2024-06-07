$(document).ready(function () {
    $('#submit').click(function () {
        var text = $('#text-input').val();
        $('#loading').text('Loading...');

        $.ajax({
            url: '/generate',
            type: 'POST',
            contentType: 'application/json',
            data: JSON.stringify({ text: text }),
            success: function (response) {
                $('#loading').text('');
                var imageBase64 = response.image_base64;
                var imageUrl = 'data:image/png;base64,' + imageBase64;
                $('#image-display').html('<img src="' + imageUrl + '" alt="Generated Image"/>');
            },
            error: function () {
                $('#loading').text('');
                $('#image-display').text('Failed to generate image.');
            }
        });
    });

    $("#tab-button-1").click(function () {
        window.location.href = "https://github.com/ahr-i";
    });

});
