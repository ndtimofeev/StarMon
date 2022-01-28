// Widgets
#include <QApplication>

// 3DCore
#include <Qt3DCore>

// 3DExtras
#include <Qt3DExtras>

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

        if ( app.arguments().size() < 2 )
            return 0;

		QFile file( app.arguments().at( 1 ) );
		if ( ! file.open( QIODevice::ReadOnly ) )
		{
            qDebug() << "Can't open " + app.arguments().at( 1 ) + " file";
            return 0;
		}

		auto doc        = QJsonDocument::fromJson( file.readAll() ).object();
		auto starsArray = doc.value( "stars" );

	
		Qt3DExtras::Qt3DWindow view;

		Qt3DCore::QEntity root;
		Qt3DExtras::QSphereMesh mesh;
		mesh.setRadius( 2 );
		mesh.setRings( 20 );
		mesh.setSlices( 20 );

		auto cameraEntity = view.camera();

		cameraEntity->lens()->setPerspectiveProjection( 45.0f, 16.0f/9.0f, 0.1f, 1000.0f );
		cameraEntity->setPosition( QVector3D( 0, 0, 20.0f ) );
		cameraEntity->setUpVector( QVector3D( 0, 1, 0 ) );
		cameraEntity->setViewCenter( QVector3D( 0, 0, 0 ) );

		auto lightEntity    = new Qt3DCore::QEntity( &root );
		auto lightTransform = new Qt3DCore::QTransform( lightEntity );
		auto light          = new Qt3DRender::QPointLight( lightEntity );
		light->setColor( "white" );
		light->setIntensity( 1 );
		lightEntity->addComponent( light );
		lightTransform->setTranslation( cameraEntity->position() );
		lightEntity->addComponent( lightTransform );

		auto camController = new Qt3DExtras::QFirstPersonCameraController( &root );
		camController->setCamera( cameraEntity );
		camController->setAcceleration( 10 );
		camController->setLinearSpeed( 50 );

		// Sphere shape data
		auto sphereMesh = new Qt3DExtras::QSphereMesh();
		sphereMesh->setRings( 20 );
		sphereMesh->setSlices( 20 );
		sphereMesh->setRadius( 2 );

		auto sphereMaterial = new Qt3DExtras::QPhongMaterial();
		sphereMaterial->setDiffuse(QColor( Qt::yellow ));

		for ( auto arg : starsArray.toArray() )
		{
			qDebug() << arg;
			auto star = arg.toObject();
			auto sphereTransform = new Qt3DCore::QTransform();
			sphereTransform->setScale(0.1f);
			sphereTransform->setTranslation(
				QVector3D(star.value( "x" ).toDouble() / 10, star.value( "y" ).toDouble() / 10 , 0.0f));

			// Sphere
			auto m_sphereEntity = new Qt3DCore::QEntity( &root );
			m_sphereEntity->addComponent(sphereMesh);
			m_sphereEntity->addComponent(sphereMaterial);
			m_sphereEntity->addComponent(sphereTransform);
		}

		view.setRootEntity( &root );
		view.show();

	return app.exec();
}
