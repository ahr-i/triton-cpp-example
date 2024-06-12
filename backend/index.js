$(document).ready(function () {
    $('#submit').click(function () {
        var text = $('#text-input').val();
        $('#loading').text('Generating image...');

        $.ajax({
            url: '/generate',
            type: 'POST',
            contentType: 'application/json',
            data: JSON.stringify({ text: text }),
            success: function (response) {
                $('#loading').text('');
                var imageBase64 = response.image_base64;
                var imageUrl = 'data:image/png;base64,' + imageBase64;
                $('#image-display').html('<img src="' + imageUrl + '" alt="Generated Image" style="max-width: 100%; border-radius: 10px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"/>');
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
